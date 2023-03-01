#include "thread_pool_server.h"



ThreadPoolServer::ThreadPoolServer()
{
	
}

ThreadPoolServer* ThreadPoolServer::create_singleton()
{
	singleton = new ThreadPoolServer();
	return singleton;
}

void ThreadPoolServer::destroy()
{
	delete singleton;
}

bool ThreadPoolServer::init()
{
	return true;
}
