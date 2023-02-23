// VulkanGameEngine.cpp : Defines the entry point for the application.
//
#include "core/string/unicode_char_utils.h"
#include "VulkanGameEngine.h"
#include "servers/configuration_server.h"
#include "servers/window_server.h"
#include "servers/creation_server.h"
#include "core/folders.h"

#include "window/viewport.h"
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

struct a
{

};

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
	auto configurationServer = creationServer->create_configuration();
	
	EventBusServer* eventBus = creationServer->create_event_bus_server();

	LoggerServer* loggerServer;
	// Begin expose scope. 
	if (auto scope = configurationServer->scope_expose())
	{
		loggerServer = creationServer->create_logger_server();
		windowServer = creationServer->create_the_window_server();
		 
	}
#ifdef _DEBUG
	loggerServer->set_log_level_cout(Logger::DEBUG);
#endif
	// Begin change scope
	// Change Default Config of Window Size
	if (auto scope = configurationServer->scope_change())
	{
		auto config = configurationServer->get_config_read("WindowServer");
		loggerServer->log_cout(windowServer, "Windwo Initted",Logger::INFO);
		UVec2 vec = { 1920,1080 };
		configurationServer->try_set_config_prop(windowServer, "WindowServer", "size", vec);
	}

	// Begin init scope.
	if (auto scope = configurationServer->scope_init())
	{
		windowServer->init();
	
	}

	windowServer->show();
	// Server Initialization must be in order. WindowServer -> Device -> ETC..
	while (!windowServer->should_close())
	{
		windowServer->handle_events();
	}


	auto strr = string32_to_string16(windowServer->get_object_name());

	windowServer->destroy();
	loggerServer->destroy();
	eventBus->destroy();
	configurationServer->destroy();
	creationServer->destroy();
	
	return 0;
}
