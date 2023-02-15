#pragma once	
#include "IEngineManager.h"
#include <vector>
#include "Common/IResource_1.h"
namespace GNF::Core
{
	//!: This interface helps to implement subflow tasking
	class IEngineManager_1 : public Common::IResource_1
	{
	public:
		IEngineManager_1(const char* name = "UNKNOWN") : Common::IResource_1(name)
		{}

		virtual ~IEngineManager_1() = default;
		
	};
}