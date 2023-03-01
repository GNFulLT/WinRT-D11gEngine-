#ifndef THREAD_POOL_SERVER_H
#define THREAD_POOL_SERVER_H

#include <taskflow/taskflow.hpp>


#include "../core/typedefs.h"
#include "../core/templates/safe_num.h"



class CreationServer;

class ThreadPoolServer
{
public:
	ThreadPoolServer();


	bool init();

	_INLINE_ static ThreadPoolServer* get_singleton()
	{
		return singleton;
	}

	void destroy();

	_INLINE_ size_t get_thread_count()
	{
		return m_execPool.num_workers();
	}

private:

private:
	tf::Executor m_execPool;
	_INLINE_ static SafeNum<uint32_t> working_thread_count = 0;


	friend class CreationServer;
	_INLINE_ static ThreadPoolServer* singleton;

	static ThreadPoolServer* create_singleton();
};


#endif