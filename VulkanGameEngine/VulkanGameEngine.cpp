// VulkanGameEngine.cpp : Defines the entry point for the application.
//

#include "core/string/unicode_char_utils.h"
#include "VulkanGameEngine.h"
#define USE_GARR_SIZE
#define USE_GARR_SIZE_TEMPLATED
#include "core/typedefs.h"
#include <GLFW/glfw3.h>
#include "core/templates/safe_num.h"
#if defined(_DEBUG) && defined(_WINDOWS)
#include <windows.h>
#define _CRTDBG_MAP_ALLOC
#include <heapapi.h>
#endif

int main()
{
#ifdef _DEBUG
#ifdef _WINDOWS

	// Notify user if heap is corrupt
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	_CrtSetDbgFlag(
		_CRTDBG_ALLOC_MEM_DF // Turn on debug allocation
		| _CRTDBG_LEAK_CHECK_DF // Leak check at program exit
		| _CRTDBG_CHECK_ALWAYS_DF // Check heap every alloc/dealloc
		| _CRTDBG_CHECK_EVERY_16_DF // Check heap every 16 heap ops
	);
	//_CrtSetBreakAlloc(47947);
#endif // _WINDOWS
#endif // _DEBUG
	int a[10] = { 5,2,3,4,5,10,10,10,10,10 };
	SafeNum<int> b = 5;
	std::atomic<int> t = 5;
	std::cout << "Hello CMake. " << b.post_mul(5) << std::endl;
	SafeNum<int> num = 5;
	return 0;
}
