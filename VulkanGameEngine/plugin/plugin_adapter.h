#ifndef PLUGIN_ADAPTER_H
#define PLUGIN_ADAPTER_H

#include "plugin_interface.h"

struct IPluginAdapter
{
	virtual ~IPluginAdapter() {}
	virtual void* adapt(void* object, GNFPluginClassDestructor df) = 0;
};

template<typename T, typename U>
struct PluginAdapter : public IPluginAdapter
{
	virtual void* adapt(void* object, GNFPluginClassDestructor df)
	{
		return new T((U*)object, df);
	}
};
#endif