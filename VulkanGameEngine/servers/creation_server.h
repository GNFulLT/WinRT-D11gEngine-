#ifndef CREATION_SERVER_H
#define CREATION_SERVER_H

#include "../core/object/object.h"
#include "window_server.h"
#include "configuration_server.h"
#include "../config/config.h"
class CreationServer : public Object
{
	OBJECT_DEF(CreationServer,Object)

public:
	CreationServer(const CreationServer&) = delete;

	CreationServer& operator=(const CreationServer&) = delete;

	static _INLINE_ CreationServer* create_creation()
	{
		return get_singleton();
	}

	static _INLINE_ CreationServer* get_singleton()
	{
		return singleton;
	}

	ConfigurationServer* create_configuration();
	WindowServer* create_the_window_server();
	static void destroy();	

private:
	CreationServer()
	{}
private:
	_INLINE_ static CreationServer* singleton;
};

#endif // CREATION_SERVER_H