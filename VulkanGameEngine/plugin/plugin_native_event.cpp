#include "plugin_native_event.h"
#include "native_include/plugin_native_event_types.h"

#include "../core/version.h"
#include "../core/event/plugin_events_defs.h"
#include "native_include/plugin_native_event_types.h"
#include <iostream>

static GNFPluginBool native_event_opener(GNFPluginPackagedArgs packaged, GNFPluginNativeEventConstructorArgs** converter)
{
	*converter = (GNFPluginNativeEventConstructorArgs*)packaged;
	return 0;
}


void GNFPluginNativeEvent::init()
{
	GNFPluginNativeEventConstructorArgs eventArg;
	GNFPluginConstructorArgs ctorArg;
	ctorArg.version.version_major = GNF_VERSION_MAJOR;
	ctorArg.version.version_minor = GNF_VERSION_MINOR;
	GNFPluginOpenPackage pck;
	pck.fNativeEventOpener = native_event_opener;
	m_resource = p_ctor(&ctorArg,&eventArg,&pck);

	if (eventArg.flag & GNFPLUGIN_EVENT_FLAG::GNFPLUGIN_ONCHANGESCOPE_EVENT)
	{
		auto func = m_lib->get_function<GNFPluginScopeChangePoint>(_STR_XDEF(GNFPLUGIN_SERVERLAYER_CHANGESCOPE_POINT_NAME));
		if (func != nullptr)
		{
			m_pointMap.emplace(GNFPLUGIN_ONCHANGESCOPE_EVENT, func);
			m_listeners.push_back(std::shared_ptr<dexode::EventBus::Listener>(EventBusServer::get_singleton()->get_listener()));
			m_listeners[m_listeners.size() - 1].get()->listen([n = this,map = &m_pointMap,handle=m_resource](const OnChangeScopeEvent& ev) {
				std::cout << "hello event";
				auto func = map->find(GNFPLUGIN_ONCHANGESCOPE_EVENT);
				if (func != map->end())
				{
					try {
						((GNFPluginScopeChangePoint)func->second)(handle, &ev);
					}
					catch (std::exception ex)
					{

					}
				}
			});
		}
	}
}	

void GNFPluginNativeEvent::destroy()
{
	p_dtor(m_resource);
}
