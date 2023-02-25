#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include "../core/object/object.h"
#include "../core/typedefs.h"
#include "logger_server.h"

#include <unordered_map>
#include <boost/signals2.hpp>
#include <boost/bind/bind.hpp>
#include <typeindex>
#include <memory>
#include <functional>
#include <boost/format.hpp>
#include <concepts>

template<typename T>
concept CanSubsribe_Concept = std::convertible_to<T, Object>;


class CreationServer;


class EventBusServer : public Object
{
	OBJECT_DEF(EventBusServer,Object)

public:
	EventBusServer() = default;
	EventBusServer(const EventBusServer&) = delete;
	EventBusServer& operator=(const EventBusServer&) = delete;

	_INLINE_ static EventBusServer* get_singleton()
	{
		return singleton;
	}

	template <CanSubsribe_Concept EventType, typename SubscriberType>
	void add_subscriber(SubscriberType* subscriber, void (SubscriberType::* handler)(const EventType&)) {
		// Check if the event type has been registered before
		auto& signal = get_signal<EventType>();
		signal.connect(boost::bind(handler, subscriber,boost::placeholders:: _1));
		if ((unsigned int)LoggerServer::get_singleton()->get_log_level_cout() <= (unsigned int)m_logLevel && (unsigned int)LoggerServer::get_singleton()->get_log_level_cout() != 0)
		{
			LoggerServer::get_singleton()->log_cout(boost::str(boost::format("[%1%] class [%2%] named object subscribed to [%3%] named event") % subscriber->get_class_name() %
				string32_to_string(subscriber->get_object_name()) % EventType::EventMetaData::name).c_str(), m_logLevel);
		}
	}

	template <typename EventType>
	void post(Object* who,const EventType& event) {
		auto& signal = get_signal<EventType>();
		if ((unsigned int)LoggerServer::get_singleton()->get_log_level_cout() <= (unsigned int)m_logLevel && (unsigned int)LoggerServer::get_singleton()->get_log_level_cout() != 0)
		{
			LoggerServer::get_singleton()->log_cout(boost::str(boost::format("[%1%] class [%2%] named object posted [%3%] named event") % who->get_class_name() %
				string32_to_string(who->get_object_name()) %  EventType::EventMetaData::name).c_str(), m_logLevel);
		}
		signal(event);
	}

	template <CanSubsribe_Concept SubscriberType,typename EventType>
	void remove_subscriber(SubscriberType* subscriber, void (SubscriberType::* handler)(const EventType&)) {
		auto& signal = get_signal<EventType>();
		signal.disconnect(boost::bind(handler, subscriber, boost::placeholders::_1));
	}

	void destroy();
private:
	template <typename EventType>
	_INLINE_ boost::signals2::signal<void(const EventType&)>& get_signal() {
		// Use type index as key to unordered map
		auto& typeIndex = typeid(EventType);
		auto it = m_signals.find(typeIndex);
		if (it != m_signals.end()) {
			return *static_cast<boost::signals2::signal<void(const EventType&)>*>(it->second.get());
		}
		else {
			auto signal = std::make_unique<boost::signals2::signal<void(const EventType&)>>();
			auto& result = *signal.get();
			m_signals[typeIndex] = std::move(signal);
			return result;
		}
	}
private:
#ifdef _DEBUG
	Logger::LOG_LEVEL m_logLevel = Logger::LOG_LEVEL::DEBUG;
#else
	Logger::LOG_LEVEL m_logLevel = Logger::LOG_LEVEL::OFF;
#endif
	std::unordered_map<std::type_index, std::unique_ptr<void,std::function<void(void*)>>> m_signals;

private:

	static EventBusServer* create_singleton();
	friend class CreationServer;
	_INLINE_ static EventBusServer* singleton;
};

#endif //EVENT_BUS_H