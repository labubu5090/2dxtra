#pragma once

#include <fstream>
#include <mutex>
#include <string>

// TEMP-SMOKE diagnostic logger (writes "./2dxtra_smoke.log").
// Lightweight append-only file trace used to confirm which hooks fire.
namespace iidxtra::smoke
{
	inline auto diag(const std::string& line) -> void
	{
		static std::mutex mutex {};
		std::lock_guard lock { mutex };
		if (auto out = std::ofstream { "2dxtra_smoke.log", std::ios::app }; out)
			out << line << '\n';
	}
}