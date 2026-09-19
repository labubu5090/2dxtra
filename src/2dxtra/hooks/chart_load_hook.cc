#include <meta.h>
#include <MinHook.h>
#include <safetyhook.hpp>
#include "../log.h"
#include "../game.h"
#include "../chart_set.h"
#include "../features/keysound_switch.h"
#include "../features/regular_speed.h"
#include "../features/chart_speed.h"
#include "../features/chart_loader.h"
#include "../features/scratch_swap.h"
#include "../features/cn_transformer.h"
#include "chart_load_hook.h"
#include "score_invalidator_hook.h"
#include "smoke_diag.h"

namespace iidxtra::chart_load_hook
{
	// Set in the outer function to tell the inner function which player the
	// chart is loading for. Used to set the relevant last_chart_id value.
	std::uint8_t next_player_id;

    // Unique string IDs for the current chart, split across both players.
	std::string last_chart_id_p1, last_chart_id_p2;

    // Note counts of the previous chart. Used by the C API.
    std::uint32_t last_chart_note_count_p1, last_chart_note_count_p2;

    // Bytes the play loader's final chart fread returned. Captured by the
    // mid-hook fired at LOAD_CHART_READ_RESULT (rax = bytes read); the byte
    // range [output, output+length) is what boot hashed into orig_hash.
    std::uint32_t last_chart_length;

    // original functions
	void* original_chart_loader_fn = nullptr;
	void* original_outer_chart_loader_fn = nullptr;

    std::unique_ptr<SafetyHookMid> g_length_hook {};

    auto capture_chart_length(SafetyHookContext& ctx) -> void
    {
        last_chart_length = static_cast<std::uint32_t>(ctx.rax);
    }

	// hook functions
	auto chart_loader_hook_fn(void* output, const char* filename, int chart_index) -> bool
	{
		// The original call below re-fills this every load; a stale length
		// left over from a previous chart must not reach load_custom_chart.
		last_chart_length = 0;

		// Call the original function early to set some stuff up for us.
		auto result = reinterpret_cast<bool (*) (void*, const char*, int)>(original_chart_loader_fn)(output, filename, chart_index);

		// Custom chart loader
		if (!chart_set::active.empty())
			chart_loader::load_custom_chart(output, chart_index);

		// Work on a copy so a mutator can shorten the stream without having to
		// preserve whatever the game already parsed. The buffer is kept around
		// between loads; it is 96 KiB and every chart load needs exactly it.
		auto static events = std::vector<bm2dx::chart_event_t> {};
		events.resize(bm2dx::CHART_EVENT_CAPACITY);
		CopyMemory(events.data(), output, bm2dx::CHART_BUFFER_BYTES);

		{
			// Mutators
            keysound_switch::mutate(next_player_id, events);
			regular_speed::mutate(next_player_id, events);
			scratch_swap::mutate(next_player_id, events);
			cn_transformer::mutate(next_player_id, events);
			chart_speed::mutate(next_player_id, events);
		}

		// A mutator that dropped events has to leave the tail zeroed rather
		// than short, since the game reads the whole fixed-size buffer back.
		events.resize(bm2dx::CHART_EVENT_CAPACITY);
		CopyMemory(output, events.data(), bm2dx::CHART_BUFFER_BYTES);

		return result;
	}

	auto chart_loader_outer_hook_fn(void* output, int player, const char* filename, int chart_index) -> bool
	{
		// Immediately disable input until the player returns to music select.
		// It should already be disabled by the music select destructor hook, but let's be safe.
		chart_set::switch_enabled = false;

		smoke::diag("chart_load: outer start (switch_enabled -> false)");

		// Reset invalid states to normal.
		score_invalidator_hook::reset(player);

		if (player == 0)
		{
			last_chart_id_p1.clear();
			next_player_id = 0;
		}
		else if (player == 1)
		{
			last_chart_id_p2.clear();
			next_player_id = 1;
		}

		auto result = reinterpret_cast<bool (*) (void*, int, const char*, int)>(original_outer_chart_loader_fn)
            (output, player, filename, chart_index);

        // Real note counts should be available now.
        if (player == 0)
            last_chart_note_count_p1 = static_cast<bm2dx::chart_buffer_t*>(output)->p1_note_count;
        else if (player == 1)
            last_chart_note_count_p2 = static_cast<bm2dx::chart_buffer_t*>(output)->p2_note_count;

        // If score saving is disabled at compile-time, instantly invalidate the score.
        #if BLOCK_ALL_SCORE_SAVE == 1
            score_invalidator_hook::invalidate(0);
            score_invalidator_hook::invalidate(1);
        #endif

		return result;
	}

	auto was_last_score_custom() -> bool
		{ return (!last_chart_id_p1.empty() || !last_chart_id_p2.empty()); }

	auto install_hook() -> void
	{
		MH_CreateHook(bm2dx::addr->LOAD_CHART_FN_A, reinterpret_cast<LPVOID>(chart_loader_outer_hook_fn), &original_outer_chart_loader_fn);
		MH_CreateHook(bm2dx::addr->LOAD_CHART_FN_B, reinterpret_cast<LPVOID>(chart_loader_hook_fn), &original_chart_loader_fn);

		if (bm2dx::addr->LOAD_CHART_READ_RESULT != nullptr)
		{
			auto hook_result = SafetyHookMid::create(
				reinterpret_cast<void*>(bm2dx::addr->LOAD_CHART_READ_RESULT),
				&capture_chart_length);
			if (hook_result.has_value())
			{
				g_length_hook = std::make_unique<SafetyHookMid>(std::move(*hook_result));
				log::print("chart_load: chart length hook installed at 0x{:X}",
					reinterpret_cast<std::uintptr_t>(bm2dx::addr->LOAD_CHART_READ_RESULT));
			}
			else
				log::print("chart_load: failed to install chart length hook");
		}
	}
}