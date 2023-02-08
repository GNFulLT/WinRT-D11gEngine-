#include "pch.h"
#include "Statistic.h"

#include <boost/chrono.hpp>

namespace GNF::Common::Statistic
{
	Statistic::Statistic()
	{
		m_timer = new boost::timer::cpu_timer();

	}
	int Statistic::GetFPS()
	{
		m_currentDelta = GetDeltaTime();
		m_frameCount++;
		if (m_currentDelta - m_lastTime > 1000.0f)
		{
			int frameCount = m_frameCount;
			m_frameCount = 0;
			m_lastTime += 1000.0f;

			return frameCount;
		}
		return -1;
	}
	double Statistic::GetDeltaTime()
	{
		return boost::chrono::duration_cast<boost::chrono::duration<double, boost::milli>>(boost::chrono::nanoseconds(m_timer->elapsed().wall)).count();
	}
	void Statistic::Reset()
	{
	}
	void Statistic::Start()
	{
		m_timer->start();
	}
	void Statistic::Stop()
	{
		m_timer->stop();
	}
	bool Statistic::IsStopped()
	{
		return m_timer->is_stopped();
	}
}