#include "event_bus_server.h"

void EventBusServer::destroy()
{
	delete singleton;
}

EventBusServer* EventBusServer::create_singleton()
{
	singleton = new EventBusServer();
	return get_singleton();
}
