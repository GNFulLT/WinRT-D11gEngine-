#include "event_bus_server.h"
#include "../core/event/plugin_events_defs.h"
#include "../servers/configuration_server.h"
#include "../plugin/native_include/plugin_excepts.h"
#include <iostream>
#include "../core/vec2.h"
static GNFPluginConfig getConfig(const char* name)
{
	auto wr = ConfigurationServer::get_singleton()->get_config_read(name);
	if (auto ptr = wr.lock())
	{
		return (GNFPluginConfig)ptr.get();
	}
	else
	{
		return nullptr;
	}
}

static GNFPluginConfigProp getConfigProp(GNFPluginConfig conf,GNFPLUGIN_CONFIG_PROP_TYPE type,const char* name,void* val)
{
	if (conf == nullptr)
		return nullptr;
	
	switch(type)
	{
	case GNFPLUGIN_CONFIG_PROP_TYPE_INT:
	{
		auto confProp = ((Config*)conf)->try_get_config_prop<int>(name);
		if (confProp == nullptr)
		{
			return nullptr;
		}
		*((int*)val) = *confProp->get_prop();
		return (GNFPluginConfigProp)confProp;
		break;
	}
	case GNFPLUGIN_CONFIG_PROP_TYPE_STRING:
	{
		auto confProp = ((Config*)conf)->try_get_config_prop<String>(name);
		if (confProp == nullptr)
		{
			return nullptr;
		}
		*((const char**)val) = confProp->get_prop()->c_str();
		return (GNFPluginConfigProp)confProp;
		break;
	}
	case GNFPLUGIN_CONFIG_PROP_TYPE_UVEC2:
	{
		auto confProp = ((Config*)conf)->try_get_config_prop<core::UVec2>(name);
		if (confProp == nullptr)
		{
			return nullptr;
		}
		UVec2 uvec;
		uvec.x = confProp->get_prop()->x;
		uvec.y = confProp->get_prop()->x;
		*((UVec2*)val) = uvec;
		return (GNFPluginConfigProp)confProp;
		break;
	}
	default:
		return nullptr;
		break;
	}

}

static void setConfigProp(GNFPluginConfigProp prop, GNFPLUGIN_CONFIG_PROP_TYPE type, void* val)
{
	if (prop == nullptr)
		return;

	switch (type)
	{
	case GNFPLUGIN_CONFIG_PROP_TYPE_INT:
		((ConfigProp<int>*)prop)->set_prop((*(int*)val));
		break;
	case GNFPLUGIN_CONFIG_PROP_TYPE_STRING:
		((ConfigProp<String>*)prop)->set_prop(*((const char**)val));
		break;
	case GNFPLUGIN_CONFIG_PROP_TYPE_UVEC2:
		((ConfigProp<core::UVec2>*)prop)->set_prop(*((core::UVec2*)val));
		break;
	default:
		break;
	}
}

dexode::EventBus::Listener* EventBusServer::get_listener()
{
	return new dexode::EventBus::Listener(m_bus);
}
EventBusServer::EventBusServer()
{
	m_bus.reset(new dexode::EventBus());
}
void EventBusServer::destroy()
{
	delete singleton;
}


void EventBusServer::trigger_OnChangeScop()
{
	auto listen = dexode::EventBus::Listener::createNotOwning(*m_bus.get());
	listen.listen([&](const OnChangeScopeEvent& ev) {
		std::cout << "HELLO EVENT TRIGGERED";
	});
	OnChangeScopeEvent scopeEvent;
	scopeEvent.fGetConfig = getConfig;
	scopeEvent.sGetConfigProps = getConfigProp;
	scopeEvent.sSetConfigProps = setConfigProp;
	auto b = m_bus->postpone(scopeEvent);
	auto c = m_bus->process();
	int ll = 5;
}