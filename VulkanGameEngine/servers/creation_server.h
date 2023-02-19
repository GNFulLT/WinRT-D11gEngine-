#ifndef CREATION_SERVER_H
#define CREATION_SERVER_H

#include "../core/object/object.h"
#include "window_server.h"
#include "../config/config.h"
class CreationServer : public Object
{
	OBJECT_DEF(CreationServer,Object)

public:
	CreationServer()
	{}
	CreationServer(const CreationServer&) = delete;
	CreationServer& operator=(const CreationServer&) = delete;


	WindowServer* CreateTHEWindowServer(const Config* config);

private:
};

#endif // CREATION_SERVER_H