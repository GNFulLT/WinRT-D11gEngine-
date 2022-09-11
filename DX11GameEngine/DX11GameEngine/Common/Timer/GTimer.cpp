#include "GTimer.h"
#include <windows.h>

namespace GNF::Common::Timer {

	GTimer::GTimer() : m_baseTime(0), m_currTime(0), m_deltaTime(0), m_isStopped(false), m_prevTime(0), m_pausedTime(0), m_stopTime(0)
	{

		__int64 countsPerSec;
		QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
		m_secondPerCount = (double)((double)1 / (double)countsPerSec);
	}
	bool GTimer::IsRunning()
	{
		return m_isStopped;
	}

	void GTimer::Tick()
	{
		if (m_isStopped)
		{
			m_deltaTime = 0;
			return;
		}
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		m_currTime = currTime;

		m_deltaTime = m_currTime - m_prevTime;

		m_prevTime = m_currTime;

		if (m_deltaTime < 0)
			m_deltaTime = 0;

	}

	float GTimer::GetDeltaTimeSeconds() const
	{
		return (float)m_deltaTime*m_secondPerCount;
	}

	void GTimer::Reset()
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		m_baseTime = currTime;
		m_prevTime = currTime;
		m_stopTime = 0;
		m_isStopped = false;

	}

	void GTimer::Stop()
	{
		if (m_isStopped)
			return;

		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		m_stopTime = currTime;
		m_isStopped = true;

	}
	int GTimer::SystemID()
	{
		return 4;
	}

	void GTimer::Start()
	{
		if (!m_isStopped)
			return;
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		//Paused time = when stopped - current time
		m_pausedTime += (currTime - m_stopTime);

		//Reset the prev time to current time
		m_prevTime = currTime;

		m_stopTime = 0;
		m_isStopped = false;
	}

	float GTimer::GetTotalTimeSeconds() const
	{
		//If we stopped
		//  * Stop Time
		//  *-----------?> Paused Time
		// (BaseTime)X
		// X----------->*----------?>S
		if (m_isStopped)
		{
			return (float)(((m_stopTime - m_baseTime) - m_pausedTime) * m_secondPerCount);
		}
		else
		{
			return (float)(((m_currTime - m_baseTime) - m_pausedTime) * m_secondPerCount);
		}
	}
}