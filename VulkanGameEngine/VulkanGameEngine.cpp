// VulkanGameEngine.cpp : Defines the entry point for the application.
//
#include "core/string/unicode_char_utils.h"
#include "VulkanGameEngine.h"
#include "servers/configuration_server.h"
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
	// 1st step is to create creation and configuration servers Creation -> Configuration
	// 2nd step is let servers expose their configurations to configuration server. This step starts with scope_expose
	// 3rd step is inject configurations to servers. This step starts with scope_init

	// In this steps servers can initialized or exposed parallel. But steps must be in order

	// To create servers we need creation server and also we need configuration server to properly config other servers.
	auto creationServer = CreationServer::create_creation();
	
	// We can inject configurations after this
	auto configurationServer = creationServer->create_configuration_server();
	
	auto eventServer = creationServer->create_event_bus_server();

	auto pluginServer = creationServer->create_plugin_server();

	// Begin expose scope. 
	if (auto scope = configurationServer->scope_expose())
	{
		windowServer = creationServer->create_the_window_server();

		pluginServer->load_all_plugins();
		pluginServer->init_all_plugins();
	}
	
	// Begin change scope
	// Change Default Config of Window Size
	if (auto scope = configurationServer->scope_change())
	{
		eventServer->trigger_OnChangeScop();
	}

	// Begin init scope.
	if (auto scope = configurationServer->scope_init())
	{
		windowServer->init();
	}
	auto cnf = configurationServer->get_config_read("WindowServer").lock();
	auto ct = cnf->try_get_config_prop<core::UVec2>("asdsad");
	windowServer->show();
	while (!windowServer->should_close())
	{
		windowServer->handle_events();
	}


	auto strr = string32_to_string16(windowServer->get_object_name());

	windowServer->destroy();
	

	pluginServer->destroy();
	eventServer->destroy();
	configurationServer->destroy();
	creationServer->destroy();
	
	
	return 0;
}
