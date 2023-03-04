// VulkanGameEngine.cpp : Defines the entry point for the application.
//
#include "core/string/unicode_char_utils.h"
#include "VulkanGameEngine.h"
#include "servers/configuration_server.h"
#include "servers/window_server.h"
#include "servers/creation_server.h"
#include "core/folders.h"
#include "core/serialize/serializable_struct.h"

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

struct SerializedStruct 
{
	int Ser1;
	int Ser2;
	SERIALIZABLE_STRUCT(PROPERTY(SerializedStruct,Ser1), PROPERTY(SerializedStruct, Ser2))
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
	
	configurationServer->read_init_configuration_file("config.json");
	RenderDevice* dev;
	EventBusServer* eventBus = creationServer->create_event_bus_server();
	LoggerServer* loggerServer;
	loggerServer = creationServer->create_logger_server();
	ThreadPoolServer* pool;
#ifdef _DEBUG
	loggerServer->set_log_level_cout(Logger::DEBUG);
#endif
	// Begin expose scope. 

	if (auto scope = configurationServer->scope_expose())
	{
		windowServer = creationServer->create_the_window_server();
		dev = creationServer->create_render_device();
		pool = creationServer->create_thread_pool();
	}

	// Begin change scope
	// Change Default Config of Window Size
	if (auto scope = configurationServer->scope_change())
	{
		auto config = configurationServer->get_config_read("WindowServer");
	}
	bool allSuccessed = true;
	// Begin init scope.
	if (auto scope = configurationServer->scope_init())
	{
		if (!windowServer->init())
		{
			allSuccessed = false;
		}
		if (!dev->init())
		{
			allSuccessed = false;
		}
	}
	if (allSuccessed)
	{
		windowServer->show();
		// Server Initialization must be in order. WindowServer -> Device -> ETC.. This is because of macos surface creation depents on glfw window
		// It is possible to add ifdef for just mac.
		tf::Taskflow flow;
		tf::Future<void> fture;
		flow.emplace([dev = dev]() {
			dev->beginFrame();
			//fture = pool->run_flow(flow);
			dev->ready_ui_data();
		});

		flow.emplace([dev = dev]() {
			dev->reset_things();

		});
		flow.emplace([dev = dev]() {
			dev->set_next_image();

		});
		
		// Pre Calls
		// It needs to calculate 
		{

		}

		while (!windowServer->should_close())
		{
			//flow.clear();
			windowServer->handle_events();

			if (dev->does_swapchain_need_validate())
			{
				dev->validate_swapchain();
			}
			// Update here

			//dev->render2();

			dev->beginFrameW();
			ThreadPoolServer::get_singleton()->run_flow(flow).wait();

			dev->fill_and_execute_cmd();
			
			dev->swapbuffers();
		}
		SerializedStruct out;
		bool ff = configurationServer->get_init_configuration("config.json", "sa", out);

		auto cnfW = get_config_read("WindowServer");
		auto cnf = cnfW.lock();
		cnf->serialize("WindowServer", "./zartzurt.json");
	}
	
	
	windowServer->destroy();
	dev->destroy();
	loggerServer->destroy();
	eventBus->destroy();
	configurationServer->destroy();
	pool->destroy();
	creationServer->destroy();
	return 0;
}
