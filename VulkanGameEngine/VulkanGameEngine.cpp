// VulkanGameEngine.cpp : Defines the entry point for the application.
//
#include "core/string/unicode_char_utils.h"
#include "VulkanGameEngine.h"
#include "servers/configuration_server.h"
#include <boost/bind.hpp>
#include "servers/window_server.h"
#include "servers/creation_server.h"

#include "window/viewport.h"
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

static WindowServer* windowServer = nullptr;

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
	
	// What is a server ?
	// Server is a singleton class that runs in program independently.
	// If you want to know anything about game flow and settings
	// You have to visit configuration server and see their config with their name
	// They are singleton so their get_class_name is unique

	// To initialize and make ready all servers there are 3 steps
	// 1 step is create creation and configuration servers Creation -> Configuration
	// 2 step is let servers expose their configurations to configuration server. This step starts with scope_expose
	// 3 step is inject configurations to servers. This step starts with scope_init

	// In this steps servers can initialized or exposed parallel. But steps must be in order

	// To create servers we need creation server and also we need configuration server to properly config other servers.
	auto creationServer = CreationServer::create_creation();
	
	// We can inject configurations after this
	auto configurationServer = creationServer->create_configuration();
	
	// Begin expose scope. 
	if (auto scope = configurationServer->scope_expose())
	{
		windowServer = creationServer->create_the_window_server();
	}
	
	// Begin change scope
	if (auto scope = configurationServer->scope_change())
	{
		auto config = configurationServer->get_config_read("WindowServer");
		if (auto windowConfig = config.lock())
		{

		}
	}

	// Begin init scope.
	if (auto scope = configurationServer->scope_init())
	{
		windowServer->init();
	}

	windowServer->show();
	while (!windowServer->should_close())
	{
		windowServer->handle_events();
	}


	auto strr = string32_to_string16(windowServer->get_object_name());

	windowServer->destroy();
	
	configurationServer->destroy();
	creationServer->destroy();
	
	return 0;
}
