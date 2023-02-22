#ifndef EVENT_BUS_SERVER
#define EVENT_BUS_SERVER


#include "../core/typedefs.h"
#include "../core/object/object.h"
#include "../plugin/native_include/plugin_events.h"
#include <dexode/EventBus.hpp>
#include <memory>
#include <functional>

class CreationServer;

class EventBusServer : public Object
{
	OBJECT_DEF(EventBusServer,Object)
public:
	
	_INLINE_ static EventBusServer* get_singleton()
	{
		return singleton;
	}

	void destroy();

	dexode::EventBus::Listener* get_listener();

	void trigger_OnChangeScop();
private:
	std::shared_ptr<dexode::EventBus> m_bus;
private:
	friend class CreationServer;
	_INLINE_ static EventBusServer* create_singleton()
	{
		singleton = new EventBusServer();
		return get_singleton();
	}

	EventBusServer();
	_INLINE_ static EventBusServer* singleton;
};


#endif // EVENT_BUS_SERVER