#ifndef CONFIG_PROP_H
#define CONFIG_PROP_H


#include <concepts>
#include "config_prop_base.h"
#include "core/typedefs.h"
#include <functional>
#include <boost/signals2.hpp>
#include "core/object/object.h"
template<typename T>
class ConfigProp : public Object
{
	OBJECT_DEF(ConfigProp, Object)
public:
	typedef boost::signals2::signal<void(const T&)> ConfigChangedSignal;
	
	ConfigProp(const T& initalValue,const typename ConfigProp<T>::ConfigChangedSignal::slot_type& ownerCallback,boost::signals2::connection* con)
	{
		m_prop = initalValue;

		// ConfigProp firstly wil call owner
		(*con) =  m_listeners.connect(ownerCallback);

	}
	ConfigProp(const ConfigProp&) = delete;
	ConfigProp& operator=(const ConfigProp&) = delete;

	inline boost::signals2::connection subscribe_changed_event(const typename ConfigProp<T>::ConfigChangedSignal::slot_type& fnc) const
	{
		return m_listeners.connect(fnc);
	}
	
	typedef T value_type;
private:
	T m_prop;
	mutable typename ConfigProp<T>::ConfigChangedSignal m_listeners;
};

template<typename T,typename U = T::value_type>
concept ConfigurationProp = std::convertible_to<T, ConfigProp<U>>;

#endif // CONFIG_PROP_H