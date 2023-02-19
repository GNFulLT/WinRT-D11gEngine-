#ifndef CONFIG_H
#define CONFIG_H

#include <unordered_map>
#include <typeindex>
#include <any>


#include "core/object/object.h"
#include "config_prop.h"
#include "core/string/string_utils.h"


template<typename T>
struct ConfigPropRegistery
{
	const char* id;
	ConfigProp<T>* conf;

	ConfigPropRegistery(const char* id, ConfigProp<T>* confProp) : id(id),conf(confProp){}

	typedef T value_type;
};

template<typename T, typename U = T::value_type>
concept ConfigPropRegistery_Concept = std::convertible_to<T, ConfigPropRegistery<U>>;

class Config : public Object
{
	OBJECT_DEF(Config, Object)
public:
	Config(const Config&) = delete;
	Config& operator=(const Config&) = delete;

	template<typename... Args>
	requires (ConfigPropRegistery_Concept<Args> && ...)
	Config(const Args&... args)
	{
		for (const auto arg : { args... }) {
			String str(arg.id);
			add_config_prop(str, (ConfigProp<typename Args::value_type...>*)arg.conf);
		}
	}

	_INLINE_ Object* get_owner()
	{			
		return m_owner;
	}

	template<typename T>
	_INLINE_ const ConfigProp<T>* const get_config_prop(const String& id) const
	{
		assert(m_propMap.find(typeid(T)) != m_propMap.end());
		assert(m_propMap.find(typeid(T))->second.find(hash_string(id)) != m_propMap.find(typeid(T))->second.end());

		return std::any_cast<std::shared_ptr<ConfigProp<T>>>(m_propMap.find(typeid(T))->second.find(hash_string(id))->second).get();
	}

	template<typename T>
	_INLINE_ boost::signals2::connection listen_config_prop(const String& id,std::function<void(const T&)> onPropChanged)
	{
		assert(m_propMap.find(typeid(T)) != m_propMap.end());
		assert(m_propMap.find(typeid(T))->second.find(hash_string(id)) != m_propMap.find(typeid(T))->second.end());

		std::any_cast<std::shared_ptr<ConfigProp<T>>>(m_propMap.find(typeid(T))->second.find(hash_string(id))->second)->subscribe_changed_event(onPropChanged);
	}
private:

	template<typename T>
	_INLINE_ void add_config_prop(String& id, ConfigProp<T>* prop)
	{
		
		// Already a unordered map created for this type
		if (std::unordered_map<std::type_index, std::unordered_map<size_t, std::any>>::iterator idMap = m_propMap.find(typeid(T));idMap != m_propMap.end())
		{
			idMap->second.emplace(hash_string(id), std::shared_ptr<ConfigProp<T>>(prop));
		}
		else
		{
			// No created map First create map
			m_propMap.emplace(typeid(T), std::unordered_map<size_t, std::any>());
			m_propMap[typeid(T)].emplace(hash_string(id),std::shared_ptr<ConfigProp<T>>(prop));
		}
		
	}

private:

	std::unordered_map<std::type_index, std::unordered_map<size_t,std::any>> m_propMap;

	Object* m_owner;

};


template<typename T>
concept Configuration = std::convertible_to<T,Config>;


#endif // CONFIG_H