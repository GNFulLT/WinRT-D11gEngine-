#pragma once
#include "FireableGEventHandler.h"
#include "GEventHandlerMember.h"

#include "Common/DeviceResources.h"

namespace GNF::Common::Event
{
	class EventHandler
	{
		public:

			//With Argument Member Function
			template<typename T, typename U>
			static FireableGEventHandler<T>* Bind(void(U::*memberFunctionToCall)(const T&), U* pThis)
			{
					return new GEventHandlerMember<T,U>(memberFunctionToCall,pThis);
			}

			//Without Argument Member Function
			template<typename U>
			static FireableGEventHandler<void>* Bind(void(U::*memberFunctionToCall)(), U* pThis)
			{
				return new GEventHandlerMember<void,U>(memberFunctionToCall,pThis);
			}
		private:
			EventHandler();
	};


}