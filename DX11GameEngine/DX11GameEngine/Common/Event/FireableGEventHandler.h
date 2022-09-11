#pragma once
#include "GEventHandlerBase.h"

namespace GNF::Common::Event
{
	template<typename T>
	class FireableGEventHandler : public GEventHandlerBase<T> 
	{
		public:
			virtual bool IsSame(GEventHandlerBase<T>* pEventHandler) = 0;
			virtual void OnEvent(T&) = 0;
	};

	template<>
	class FireableGEventHandler<void> : public GEventHandlerBase<void>
	{	
		public:
			virtual bool IsSame(GEventHandlerBase<void>* pEventHandler) = 0;
			virtual void OnEvent() = 0;
	};
}
