// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

#ifdef _DEBUG
#pragma comment(lib,"debug/DirectXTex.lib")
#else if
#pragma comment(lib,"DirectXTex.lib")
#endif

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.
