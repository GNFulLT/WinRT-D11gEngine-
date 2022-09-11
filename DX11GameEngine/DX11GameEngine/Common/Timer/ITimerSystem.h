#pragma once
#include "Common/System/ISystem.h"

namespace GNF::Timer
{
	class ITimerSystem : public Common::ISystem
	{
		public:
			virtual ~ITimerSystem() {}
			virtual int SystemID() = 0;
			virtual float GetTotalTimeSeconds() const = 0;
			virtual float GetDeltaTimeSeconds() const = 0;
			virtual void Reset() = 0;
			virtual void Start() = 0;
			virtual void Stop() = 0;
			virtual void Tick() = 0;
			virtual bool IsRunning() = 0;
	};
}