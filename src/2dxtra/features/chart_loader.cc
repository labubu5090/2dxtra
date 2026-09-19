#include "../log.h"
#include "../game.h"
#include "../chart_set.h"
#include "../urafumen.h"
#include "../hooks/chart_load_hook.h"
#include "chart_loader.h"

namespace iidxtra::chart_loader
{
	auto load_custom_chart(void* output, const int) -> bool
	{
		if (chart_set::active.empty() || chart_set::cache() == nullptr)
			return false;

		// The play loader just read the original chart into `output`; its
		// byte length was captured right after the final fread. Hash the
		// exact bytes the read returned, boot analyzed the same range.
		auto const len = chart_load_hook::last_chart_length;
		if (len == 0 || len > bm2dx::CHART_BUFFER_BYTES)
			return false;

		auto const orig_hash = urafumen::sha256_hex(
			static_cast<std::uint8_t*>(output), len);
		if (orig_hash.empty())
			return false;

		// Resolve (music_id, difficulty) from the hash and pull the active
		// set's mutated chart into `output`, decompressing it in place.
		int music_id = -1, difficulty = -1;
		auto const pulled = database::pull_by_orig_hash(
			chart_set::cache(), chart_set::custom.at(chart_set::active).id,
			orig_hash, static_cast<std::uint8_t*>(output),
			bm2dx::CHART_BUFFER_BYTES, &music_id, &difficulty);

		if (!pulled.has_value())
			return false;

		// Save the chart ID for later use in stage_result_hook.
		(chart_load_hook::next_player_id == 0 ?
			chart_load_hook::last_chart_id_p1:
			chart_load_hook::last_chart_id_p2) = pulled->hash;

		// If the note count changed, display the difference à la 2dxplus.
		auto const original_notes = [&] {
			auto const loader_index = chart_set::loader_index_of(difficulty);
			auto const music_it = chart_set::stock.music.find(music_id);
			if (!loader_index.has_value() || music_it == chart_set::stock.music.end())
				return pulled->notes;
			auto const chart_it = music_it->second.charts.find(loader_index.value());
			return chart_it == music_it->second.charts.end()
				? pulled->notes : chart_it->second.notes;
		}();

		if (pulled->notes != original_notes)
			log::print("[{}] P{}: {} + {} -> {} notes", chart_set::active,
				chart_load_hook::next_player_id + 1, original_notes,
				pulled->notes - original_notes, pulled->notes);

		return true;
	}
}
