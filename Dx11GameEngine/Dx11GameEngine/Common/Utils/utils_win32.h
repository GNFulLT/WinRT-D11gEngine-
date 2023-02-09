#pragma once


#ifdef _DEBUG
#define GThrowIfFailed(hr) GNF::Common::Utils::ThrowIfFailed(hr,__FUNCTION__,__FILE__,__LINE__)
#define GCheckAnyError() GNF::Common::Utils::CheckAnyError(__FUNCTION__,__FILE__,__LINE__)
#else if
#define GThrowIfFailed(hr) GNF::Common::Utils::ThrowIfFailed(hr,__FUNCTION__,__FILE__,__LINE__)
#define GCheckAnyError()  GNF::Common::Utils::CheckAnyError(__FUNCTION__,__FILE__,__LINE__)
#endif

#include <Windows.h>

namespace GNF::Common::Utils
{
	void ThrowIfFailed(HRESULT hr, const char* functionName, const char* fileName, int lineNumber);

	void CheckAnyError(const char* functionName, const char* fileName, int lineNumber);
}