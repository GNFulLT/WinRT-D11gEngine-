#ifndef PLUGIN_NATIVE_EVENT
#define PLUGIN_NATIVE_EVENT

#include "plugin_native.h"
#include <vector>
#include <boost/signals2.hpp>
#include "../servers/event_bus_server.h"
#include <unordered_map>
#include "native_include/plugin_native_event_types.h"

static GNFPluginBool native_event_opener(GNFPluginPackagedArgs packaged, GNFPluginNativeEventConstructorArgs* converter);

class GNFPluginNativeEvent : public GNFPluginNative
{
	OBJECT_DEF(GNFPluginNativeEvent,GNFPluginNative)
public:
	GNFPluginNativeEvent(const String& name,DynamicLibrary* lib, GNFPluginClassConstructor ctor, GNFPluginClassDestructor dtor, GNFPluginExitFunc exitFunc, GNFPluginVersion vers) :
		GNFPluginNative(name,lib,ctor, dtor, exitFunc, vers)
	{}

	virtual void init() override;

	virtual void destroy() override;

private:
	std::unordered_map<GNFPLUGIN_EVENT_FLAG,void*> m_pointMap;
	std::vector<std::shared_ptr<dexode::EventBus::Listener>> m_listeners;
};

#endif