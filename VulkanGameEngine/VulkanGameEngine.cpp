// VulkanGameEngine.cpp : Defines the entry point for the application.
//



#include "core/string/unicode_char_utils.h"
#include "VulkanGameEngine.h"
#include "servers/configuration_server.h"
#include <boost/bind.hpp>
#include <codecvt>



#include <GLFW/glfw3.h>
#include "core/templates/safe_num.h"
#if defined(_DEBUG) && defined(_WINDOWS)
#include <windows.h>
#define _CRTDBG_MAP_ALLOC
#include <heapapi.h>
#endif

#define USE_GARR_SIZE
#define USE_GARR_SIZE_TEMPLATED
#define USE_DEBUG_ALLOCATION
#define USE_DEBUG_ALLOCATION_IMPLICIT
#include "core/typedefs.h"

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
	ConfigurationServer serv;
	boost::function<void(int)> f1 = [](int changed)
	{

	};
	boost::signals2::connection con;
	ConfigProp<int>* b = new ConfigProp<int>{5,f1,&con};
	ConfigPropRegistery t("sadsa", b);
	
	Config conf(t);
	auto prop = conf.get_config_prop<int>("sadsa");
	auto strr = std::wstring_convert<
		std::codecvt_utf8_utf16<char32_t>, char32_t>{}.to_bytes(prop->get_class_name());

	std::cout << strr.c_str();

	return 0;
}
