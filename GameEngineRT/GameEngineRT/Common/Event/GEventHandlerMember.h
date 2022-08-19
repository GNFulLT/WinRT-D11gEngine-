#pragma once

#include "GEventHandlerBase.h"
#include "Common/DeviceResources.h"

namespace GNF::Common::Event
{

	//With One Argument
	template<typename T,typename U>
	class GEventHandlerMember : public FireableGEventHandler<T>
	{
		public:
			GEventHandlerMember(void(U::* memberFunctionToCall)(const T&), U* pThis) : m_memberFunctionToCall(memberFunctionToCall),m_pCallerInstance(pThis)
			{}

			virtual bool IsSame(GEventHandlerBase<T>* pEventHandler)
			{
				if (IsSameType(pEventHandler))
					return false;

				GEventHandlerMember<T, U>* pHandlerCasted = dynamic_cast<GEventHandlerMember<T, U>*>(pEventHandler);

				return m_memberFunctionToCall == pHandlerCasted->m_memberFunctionToCall && m_pCallerInstance == pHandlerCasted->m_pCallerInstance;
			}

			virtual void OnEvent(T& arg)
			{
				if (m_pCallerInstance)
				{
					(m_pCallerInstance->*m_memberFunctionToCall)(arg);
				}
			}

		private:
			U* m_pCallerInstance;
			void(U::* m_memberFunctionToCall)(const T&);
	};

	//Without Argument
	template<typename U>
	class GEventHandlerMember<void,U> : public FireableGEventHandler<void>
	{
		public:
			GEventHandlerMember(void(U::* memberFunctionToCall)(), U* pThis) : m_memberFunctionToCall(memberFunctionToCall),m_pCallerInstance(pThis)
			{}

			virtual bool IsSame(GEventHandlerBase<void>* pEventHandler)
			{
			if (IsSameType(pEventHandler))
				return false;

			GEventHandlerMember<U>* pHandlerCasted = dynamic_cast<GEventHandlerMember<U>*>(pEventHandler);

			return m_memberFunctionToCall == pHandlerCasted->m_memberFunctionToCall && m_pCallerInstance == pHandlerCasted->m_pCallerInstance;
			}

			
			virtual void OnEvent() 
			{
				(m_pCallerInstance->*m_memberFunctionToCall)();
			}

		private:
			U* m_pCallerInstance;
			void(U::* m_memberFunctionToCall)();
	};
}