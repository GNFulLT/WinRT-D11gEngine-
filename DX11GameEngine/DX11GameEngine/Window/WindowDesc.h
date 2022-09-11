#pragma once
#include <Windows.h>

namespace GNF::Window
{
	struct WindowDesc
	{
		const wchar_t* windowClassName;
		const wchar_t* windowTitle;
		UINT height;
		UINT width;
	};
}