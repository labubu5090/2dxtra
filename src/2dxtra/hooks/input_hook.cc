#include <bitset>
#include <fstream>
#include <mutex>
#include <MinHook.h>
#include "../log.h"
#include "../game.h"
#include "../input.h"
#include "../gui/gui.h"
#include "input_hook.h"

#include <windows.h>

namespace iidxtra::input_hook
{
	// TEMP-SMOKE diagnostic logger (writes "./2dxtra_smoke.log").
	namespace
	{
		std::mutex g_diag_mutex {};
		int g_diag_count = 0;

		auto diag(const std::string& line) -> void
		{
			std::lock_guard lock { g_diag_mutex };
			if (g_diag_count++ >= 2000)
				return;
			if (auto out = std::ofstream { "2dxtra_smoke.log", std::ios::app }; out)
				out << line << '\n';
		}
	}

	void* (*original_input_fn) (bm2dx::InputManagerIIDX*) = nullptr;

	// How long the second EFFECT tap may arrive after the first, in milliseconds.
	auto constexpr double_tap_window_ms = 400;

	auto input_hook_fn(bm2dx::InputManagerIIDX* a1) -> void*
	{
		auto static old_state = bm2dx::input_t {};
		auto static frame = 0;
		auto static was_effect_down = false;
		auto static last_press_ms = 0ULL;

		// The poll fn's input data lives in two disjoint regions of the object:
		// the button bitfields at the front and the turntable slots further in.
		// The bytes between them are the game's own list/map headers - copying
		// those back corrupts the input manager and crashes on the next poll,
		// so only the two named windows are ever touched.
		auto constexpr buttons_size = bm2dx::INPUT_BUTTON_BYTES;
		auto constexpr turntable_size = bm2dx::INPUT_TURNTABLE_BYTES;

		auto const effect_bit = static_cast<std::size_t>(bm2dx::button::EFFECT);

		if (frame++ == 0)
			diag("input_hook_fn first call, buttons=" + std::to_string(a1->data.buttons));

		// menu is visible -- lock inputs from the game
		if (gui::visible)
		{
			CopyMemory(&old_state.buttons, &a1->data.buttons, buttons_size);
			CopyMemory(&old_state.p1_turntable, &a1->data.p1_turntable, turntable_size);
		}

		// get the new inputs and feed them to the menu
		auto const result = original_input_fn(a1);

		if (gui::visible)
		{
			CopyMemory(&input::menu.buttons, &a1->data.buttons, buttons_size);
			CopyMemory(&input::menu.p1_turntable, &a1->data.p1_turntable, turntable_size);

			auto static last_menu_buttons = 0u;
			if (a1->data.buttons != last_menu_buttons)
			{
				last_menu_buttons = a1->data.buttons;
				diag("menu buttons=0x" + [&] {
					char buf[16] = {};
					snprintf(buf, sizeof buf, "%x", a1->data.buttons);
					return std::string(buf);
				}());
			}
		}

		// toggle the gui state on a double tap: two fresh EFFECT presses
		// within the window. Uses wall-clock time so the detection does not
		// depend on config->target_fps (which may be 0 / mislocated on 34).
		{
			auto const effect_down = std::bitset<32>(a1->data.buttons).test(effect_bit);

			if (effect_down && !was_effect_down)
			{
				auto const now = GetTickCount64();
				diag("EFFECT fresh press now=" + std::to_string(now)
					+ " last=" + std::to_string(last_press_ms)
					+ " gap=" + std::to_string(last_press_ms ? now - last_press_ms : 0));

				if (last_press_ms != 0 && now - last_press_ms <= double_tap_window_ms)
				{
					// second tap occurred within the window
					// check if we're allowed to open the gui
					if (!gui::visible && gui::play_lock_state && bm2dx::play_session && bm2dx::play_session->in_gameplay)
						log::print("Menu is currently unavailable");
					else
						gui::visible = !gui::visible;

					diag(std::string("gui::visible -> ") + (gui::visible ? "true" : "false"));

					last_press_ms = 0;
				}
				else
				{
					// now waiting for the next tap
					last_press_ms = now;
				}
			}

			was_effect_down = effect_down;
		}

		// restore old input state
		if (gui::visible)
		{
			CopyMemory(&a1->data.buttons, &old_state.buttons, buttons_size);
			CopyMemory(&a1->data.p1_turntable, &old_state.p1_turntable, turntable_size);
		}

		return result;
	}

	auto install_hook() -> void
	{
		auto const status = MH_CreateHook(bm2dx::addr->INPUT_POLL_FN, reinterpret_cast<LPVOID>(input_hook_fn), (void**) &original_input_fn);
		diag("MH_CreateHook INPUT_POLL_FN status=" + std::to_string(static_cast<int>(status)));
	}
}