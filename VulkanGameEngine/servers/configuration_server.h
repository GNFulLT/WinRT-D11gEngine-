#ifndef CONFIGURATION_SERVER_H
#define CONFIGURATION_SERVER_H

#include <typeindex>
#include <unordered_map>
#include <any>
#include <cassert>

#include "../core/object/object.h"
#include "../config/config.h"
#include "../core/scope.h"
#include "../core/string/string_utils.h"


class CreationServer;

// Basically It is like just templated Dependency Container
// First server that is up. If want to change any server configuration. Change from this.
class ConfigurationServer : public Object
{
	OBJECT_DEF(ConfigurationServer,Object)
public:

	static _INLINE_ ConfigurationServer* get_singleton()
	{
		return singleton;
	}

public:
	ConfigurationServer(const ConfigurationServer&) = delete;
	ConfigurationServer& operator=(const ConfigurationServer&) = delete;

	_IMP_RETURN_ _INLINE_ Scope scope_init()
	{
		return Scope(std::bind(&ConfigurationServer::scope_init_begins,this),std::bind(&ConfigurationServer::scope_init_ends,this));
	}

	_IMP_RETURN_ _INLINE_ Scope scope_change()
	{
		return Scope(std::bind(&ConfigurationServer::scope_change_begins, this), std::bind(&ConfigurationServer::scope_change_ends, this));
	}

	_IMP_RETURN_ _INLINE_ Scope scope_expose()
	{
		return Scope(std::bind(&ConfigurationServer::scope_expose_begins, this), std::bind(&ConfigurationServer::scope_expose_ends, this));
	}

	_INLINE_ bool is_in_init_scope()
	{
		return m_init_scope_started;
	}

	_INLINE_ bool is_in_expose_scope()
	{
		return m_expose_scope_started;
	}

	_INLINE_ void register_config(const String& configName, Config* config)
	{
		assert(m_container.find(hash_string(configName)) == m_container.end());

		m_container.emplace(hash_string(configName),std::shared_ptr<Config>(config));
	}

	_INLINE_ const std::weak_ptr<Config> get_config_read(const String& configName)
	{
		assert(m_container.find(hash_string(configName)) != m_container.end());

		return std::any_cast<std::shared_ptr<Config>>(m_container[hash_string(configName)]);
	}


	static void destroy();

private:
	friend class CreationServer;

	static _INLINE_ ConfigurationServer* create_singleton()
	{
		singleton = new ConfigurationServer();
		return singleton;
	}

	ConfigurationServer()
	{}

	std::unordered_map<size_t, std::any> m_container;
	bool m_init_scope_started = false;
	bool m_init_scope_finished = false;

	bool m_expose_scope_started = false;
	bool m_expose_scope_finished = false;

	bool m_change_scope_started = false;
	bool m_change_scope_finished = false;
private:
	void scope_expose_begins();
	void scope_expose_ends();

	void scope_init_begins();
	void scope_init_ends();

	void scope_change_begins();
	void scope_change_ends();
	// Statics
private:
	_INLINE_ static ConfigurationServer* singleton;

};


#endif // CONFIGURATION_SERVER_H