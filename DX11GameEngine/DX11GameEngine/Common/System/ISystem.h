#pragma once


namespace GNF::Common
{
	class ISystem
	{
		public:
			virtual int SystemID() = 0;
			virtual ~ISystem() {};
	};
}