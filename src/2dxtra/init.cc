#include <filesystem>
#include <MinHook.h>
#include "chart_set.h"
#include "score_set.h"
#include "database.h"
#include "log.h"
#include "hooks/input_hook.h"
#include "hooks/renderer_hook.h"
#include "hooks/network_hook.h"
#include "hooks/chart_load_hook.h"
#include "hooks/chart_analyze_hook.h"
#include "hooks/dan_select_hook.h"
#include "hooks/music_select_hook.h"
#include "hooks/stage_result_hook.h"
#include "hooks/mselect_genre_hook.h"
#include "hooks/mdata_load_hook.h"
#include "hooks/boot_screen_hook.h"
#include "hooks/renderer_hook.h"
#include "hooks/reset_state_hook.h"
#include "hooks/result_title_hook.h"
#include "hooks/score_invalidator_hook.h"
#include "hooks/card_out_hook.h"
#include "hooks/play_field_load_hook.h"
#include "hooks/judge_timing_hook.h"
#include "hooks/fast_slow_hook.h"
#include "hooks/attract_randomizer_hook.h"
#include "features/autoplay.h"
#include "features/unrandomizer.h"
#include "features/autoretry.h"
#include "features/chart_speed.h"

namespace iidxtra
{
	// __try needs a leaf function: init() has objects that require unwinding.
	static auto arm_input_hook() -> bool
	{
		__try
		{
			input_hook::install_hook();
			return true;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			return false;
		}
	}

	auto init(LPVOID param) -> DWORD
	{
		auto const module = static_cast<HMODULE>(param);

		MH_Initialize();

		// Open the chart database next to the module.
		wchar_t module_path[MAX_PATH] = {};
		GetModuleFileNameW(module, module_path, MAX_PATH);

		auto const cwd = std::filesystem::path { module_path }.remove_filename();
		auto const db_path = (cwd / "2dxtra.sqlite").string();
		auto const db = database::open(db_path.c_str());

		chart_set::init(db);

		// Allocate memory.
		score_set::init();

		// Initialize hooks.
		// Score-table hooks reject themselves at runtime when the tables are
		// unported (bm2dx::scores[] is null on 2026091600-010), so installing
		// them is always safe.
		fast_slow_hook::install_hook();
		fast_slow_hook::set_enabled(true);

		// Menu: open/close with two quick EFFECT taps. The ImGui renderer can
		// only be armed once the game owns a D3D9 device, so a background
		// thread polls the candidate device cell until it becomes usable
		// (all validated safely inside install_hook).
		if (!arm_input_hook())
			log::print("Menu input hook could not be installed");

		// Autoplay (auto play selector). Only runs when a valid GAME_STATE is
		// present (enables safe player detection); autoplay itself stays inert
		// until toggled from the menu.
		autoplay::install_hook();

		// Chart sets: scene detection is ported (MUSIC_SELECT_CTOR /
		// DAN_SELECT_CTOR confirmed). SCENE_DTOR remains unported so the
		// switch is also cleared when a chart loads, which is safe.
		music_select_hook::install_hook();
		dan_select_hook::install_hook();

		// Stock population: populate_default_set fills chart_set::stock
		// from the music_data blob via GET_MUSIC_DATA (0x939100).
		// MDATA_LOAD_FN (0x939F00) fires after /data/info/0/music_data.bin
		// is fully loaded at boot. The hook also arms the renderer via
		// renderer_hook::install_hook (redundant with the boot thread but
		// guarded against double-install).
		mdata_load_hook::install_hook();

		// Chart loading: LOAD_CHART_FN_A (0x8142D0) is the outer loader
		// (output, player, filename, chart_index); LOAD_CHART_FN_B
		// (0x8141B0) is the inner one (output, filename, chart_index) that
		// reads the .1 chart into a scratch buffer.
		chart_load_hook::install_hook();

		// Chart sets: the game's boot chart analyze pass (0x813040) reads
		// each .1 chart once per song; install() mid-hooks the fread and
		// replays it per chart set (Kiraku/Kichiki/All-Scratch) to build the
		// charts table. Boiler: first boot with an empty cache builds slowly.
		chart_analyze_hook::install(db);

		// Genre note-delta line in music select (the N+/H+/A+ text). This was
		// the acceptance test; the offsets (MSELECT_GENRE_A/B/C) are
		// confirmed on 2026091600-010.
		mselect_genre_hook::install_hook();

		// Result screen: writes the active chart set name + chart speed under
		// the artist (RESULT_ARTIST_FN 0x906E70 confirmed).
		result_title_hook::install_hook();

		// Stage result: stores custom-chart scores (STAGE_RESULT_FN 0x914080
		// confirmed). Score tables are null on this build, so it only records
		// custom scores and skips the game table backup.
		stage_result_hook::install_hook();

		// Boot/title model rainbow text (BOOT_MODEL_TEXT_CALL 0x926F7D and
		// TITLE_MODEL_TEXT_CALL 0x91D68C confirmed).
		boot_screen_hook::install_hook();

		// Attract mode song/difficulty randomizer (ATTRACT_SELECT_FN
		// 0x8B1610 confirmed).
		attract_randomizer_hook::install_hook();

		// Score submission gatekeeping. Individual hooks reject themselves on
		// unported offsets; the flags are used by autoplay/scoring anyway.
		network_hook::install_hook();
		score_invalidator_hook::install_hook();

		// Un-randomizer (APPLY_RANDOM_FN 0x80FB60 confirmed; the random
		// table address is unported so the feature stays inert until toggled).
		unrandomizer::install_hook();

		// Chart speed (LOAD_AUDIO_FN 0xA9A290 / GET_SOUND_ENTRY_FN 0xA99F30
		// confirmed). Disabled here until its layout is re-validated on the
		// machine this runs on; the hook rejects itself while the offsets are
		// null.
		chart_speed::install_hook();

		// Auto retry / target display: reject themselves while the ghost,
		// graph, retry and fail offsets are unported.
		autoretry::install_hook();

		// Timing modifier (TIMING_HOOK_FN), card-out score revert
		// (CARD_OUT_VFUNC), reset-state cleanup (RESET_STATE_FN) and play
		// field CN states (PLAY_FIELD_LOAD): each install rejects itself on
		// unported offsets, so calling all of them is safe.
		judge_timing_hook::install_hook();
		card_out_hook::install_hook();
		reset_state_hook::install_hook();
		play_field_load_hook::install_hook();

		auto const menu_thread = CreateThread(nullptr, 0, [](LPVOID) -> DWORD {
			for (auto attempt = 0; attempt < 900; ++attempt)
			{
				if (renderer_hook::install_hook())
				{
					log::init("Renderer hook installed - press EFFECT twice for the menu");
					return 0UL;
				}
				Sleep(100);
			}
			return 0UL;
		}, nullptr, 0, nullptr);
		if (menu_thread)
			CloseHandle(menu_thread);

		// Enable all hooks.
		MH_EnableHook(MH_ALL_HOOKS);

#ifndef NDEBUG
		// Wait for detach signal.
		while (true)
		{
			if (GetAsyncKeyState(VK_LSHIFT) && GetAsyncKeyState(VK_F10))
			{
				// Revert to default before unloading.
				chart_set::revert();
				break;
			}

			Sleep(10);
		}

		// Free stock score storage.
		score_set::uninit();

		// Hooks that require extra setup.
        renderer_hook::uninstall_hook();
		card_out_hook::uninstall_hook();

		database::close(db);

		MH_Uninitialize();
		FreeLibraryAndExitThread(static_cast<HMODULE>(param), 0);
#endif

		return 0;
	}
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		if (!bm2dx::resolve())
			return FALSE;

		bm2dx::set_soft_rev('E');

		CreateThread(nullptr, 0, iidxtra::init, module, 0, nullptr);
	}

	return TRUE;
}