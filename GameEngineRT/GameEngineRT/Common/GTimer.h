#pragma once

namespace GNF::Common {

	class GTimer
	{
	public:
		GTimer();
		float GetTotalTimeSeconds() const;
		float GetDeltaTimeSeconds() const;
		void Reset();
		void Start();
		void Stop();
		void Tick();
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