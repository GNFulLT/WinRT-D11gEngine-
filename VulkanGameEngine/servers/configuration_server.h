#ifndef CONFIGURATION_SERVER_H
#define CONFIGURATION_SERVER_H

#include <typeindex>
#include <unordered_map>
#include <any>
#include <cassert>

#include "../core/object/object.h"
#include "../config/config.h"
#include "../core/scope.h"



// Basically It is like just templated Dependency Container
// First server that is up. If want to change any server configuration. Change from this.
class ConfigurationServer : public Object
{
	OBJECT_DEF(ConfigurationServer,Object)
public:
	_IMP_RETURN_ _INLINE_ Scope scope_init()
	{
		return Scope(std::bind(&ConfigurationServer::scope_int_begins,this),std::bind(&ConfigurationServer::scope_int_ends,this));
	}

	template<Configuration T>
	_INLINE_ const T* const get_config_read()
	{
		if (const auto i = m_container.find(typeid(T)))
		{
			assert(i == m_container.end());
			
			return std::any_cast<T*>(i->second);
		}
	}


private:
	std::unordered_map<std::type_index, std::any> m_container;
	bool m_init_scope_started = false;
	bool m_init_scope_finished = true;
	bool m_inited = false;

private:
	void scope_int_begins();
	void scope_int_ends();
	// Statics
private:
	static ConfigurationServer* singleton;

};


#endif // CONFIGURATION_SERVER_H