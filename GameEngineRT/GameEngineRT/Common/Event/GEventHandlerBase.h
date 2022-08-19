#pragma once


namespace GNF::Common::Event
{
	template<typename T>
	class GEventHandlerBase
	{
		public:
			GEventHandlerBase(){}
			virtual ~GEventHandlerBase(){}
			virtual bool IsSame(GEventHandlerBase<T>* pEventHandler) = 0;

			bool IsSameType(GEventHandlerBase<T>* pEventHandler)
			{
				if (!pEventHandler || typeid(*this) == typeid(*pEventHandler))
				{
					return false;
				}
				return true;
			}
	};
}