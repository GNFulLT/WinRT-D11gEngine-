#pragma once

#include <unordered_map>
#include <Windows.h>

namespace GNF {
	class WindowsMessageMap {
		public:
			WindowsMessageMap();
			std::string operator()(DWORD message, LPARAM lParam, WPARAM wParam) const;
		private:
			std::unordered_map<DWORD, std::string> map;
	};
}
