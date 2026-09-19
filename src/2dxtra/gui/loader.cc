#include <imgui.h>
#include <string>
#include <utility>
#include "../chart_set.h"
#include "../game.h"
#include "../hooks/smoke_diag.h"
#include "loader.h"

namespace iidxtra::gui
{
	// Last message state logged by render_loader: 0 = enabled, 1 = in music
	// select message, 2 = unavailable-on-this-build message.
	inline int g_last_msg_state = -1;

    auto render_loader() -> void
    {
		auto const flags = chart_set::switch_enabled ? ImGuiSelectableFlags_None: ImGuiSelectableFlags_Disabled;

		if (!ImGui::CollapsingHeader("Loader", ImGuiTreeNodeFlags_DefaultOpen))
			return;

		ImGui::Text("Available chart sets:");
		ImGui::Indent(10);
			if (ImGui::Selectable("Default", chart_set::active.empty(), flags))
				chart_set::revert();

			for (auto const& [name, set]: chart_set::custom)
			{
				if (ImGui::Selectable(name.c_str(), chart_set::active == name, flags))
					chart_set::set_active(name);

				ImGui::SameLine(300); ImGui::Text("%llu charts", set.count);
			}
		ImGui::Unindent(10);

		if (!chart_set::switch_enabled)
		{
			if (bm2dx::addr->MUSIC_SELECT_CTOR == nullptr || bm2dx::addr->DAN_SELECT_CTOR == nullptr)
			{
				if (std::exchange(g_last_msg_state, 2) != 2)
					smoke::diag("loader: \"unavailable on this build\" shown (ms_ctor=0x.." +
						std::to_string(reinterpret_cast<std::uintptr_t>(bm2dx::addr->MUSIC_SELECT_CTOR) & 0xFFFF) +
						" dan_ctor=0x.." + std::to_string(reinterpret_cast<std::uintptr_t>(bm2dx::addr->DAN_SELECT_CTOR) & 0xFFFF) + ")");
				ImGui::TextColored({0.5f, 0.2f, 0.2f, 1.f}, "Chart sets are unavailable on this build (scene detection not ported)");
			}
			else
			{
				if (std::exchange(g_last_msg_state, 1) != 1)
					smoke::diag("loader: \"only in music select\" shown");
				ImGui::TextColored({0.5f, 0.2f, 0.2f, 1.f}, "The active chart set can only be changed in music select");
			}
		}
		else if (std::exchange(g_last_msg_state, 0) != 0)
			smoke::diag("loader: switching enabled");
    }
}