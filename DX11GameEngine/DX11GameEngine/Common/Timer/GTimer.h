#pragma once

#include "ITimerSystem.h"

namespace GNF::Common::Timer {

	class GTimer : public GNF::Timer::ITimerSystem
	{
	public:
		GTimer();
		~GTimer()
		{
			int a = 5;
		}
		float GetTotalTimeSeconds() const override;
		float GetDeltaTimeSeconds() const override;
		void Reset() override;
		void Start() override;
		void Stop() override;
		void Tick() override;
		bool IsRunning() override;
		virtual int SystemID() override;
	private:
		double m_secondPerCount;
		double m_deltaTime;
		unsigned __int64 m_fpsTimer;
		unsigned __int64 m_baseTime;
		unsigned __int64 m_pausedTime;
		unsigned __int64 m_stopTime;
		unsigned __int64 m_prevTime;
		unsigned __int64 m_currTime;

		bool m_isStopped;

	};

}