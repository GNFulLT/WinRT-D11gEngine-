#pragma once
#include "FireableGEventHandler.h"
#include <vector>
#include <list>

namespace GNF::Common::Event
{
	template<typename T>
	class EventBase
	{
		public:
			EventBase& operator=(const EventBase&) = delete;


			virtual ~EventBase()
			{
				for (const auto& iter : m_eventHandlers)
				{
					FireableGEventHandler<T>* pHandler = iter;
					if (pHandler)
					{
						delete pHandler;
						pHandler = 0;
					}
				}
				m_eventHandlers.clear();
			}


			EventBase<T>& operator += (FireableGEventHandler<T>* pHandlerToAdd)
			{
				if (!pHandlerToAdd)
					return *this;

				m_eventHandlers.push_back(pHandlerToAdd);

				return *this;
			}

			EventBase<T>& operator -= (FireableGEventHandler<T>* pHandlerTorRemove)
			{
				if (!pHandlerTorRemove)
					return *this;


				for (auto iter = m_eventHandlers.begin(); iter != m_eventHandlers.end(); iter++)
				{
					FireableGEventHandler<T>* pHandler = *iter;
					if (pHandlerTorRemove->IsSame(pHandler))
					{
						if (pHandler)
						{
							delete pHandler;
							pHandler = 0;
						}

						m_eventHandlers.erase(iter);
						break;
					}
				}
				if (pHandlerTorRemove)
				{
					delete pHandlerTorRemove;
					pHandlerTorRemove = 0;
				}
				return *this;
			}

	    protected:
			std::list< FireableGEventHandler<T>* > m_eventHandlers;

	};



	template<typename T>
	class Event : public EventBase<T>
	{
		public:

			void operator()(T& eventData)
			{
				for (const auto& iter : this->m_eventHandlers)
				{
					FireableGEventHandler<T>* pHandler = iter;
					if (pHandler)
					{
					pHandler->OnEvent(eventData);  
					}
				}
			}
	};

	template<>
	class Event<void> : public EventBase<void>
	{
	public:

		void operator()()
		{
			for (const auto& iter : m_eventHandlers)
			{
				FireableGEventHandler<void>* pHandler = iter;
				if (pHandler)
				{
					pHandler->OnEvent();
				}
			}
		}
	};
}