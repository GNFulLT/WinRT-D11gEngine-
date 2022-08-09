#include "pch.h"
#include "GTimer2.h"

namespace GNF::Common
{
	GTimer2::GTimer2()
	{
		start = std::chrono::high_resolution_clock::now();
		stop = std::chrono::high_resolution_clock::now();
	}

	double GTimer2::GetMilisecondsElapsed()
	{
		if (isRunning)
		{
			auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start);
			return elapsed.count();
		}
		else
		{
			auto elapsed = std::chrono::duration<double, std::milli>(stop - start);
			return elapsed.count();
		}
	}

	void GTimer2::Restart()
	{
		isRunning = true;
		start = std::chrono::high_resolution_clock::now();
	}

	bool GTimer2::Stop()
	{
		if (!isRunning)
			return false;
		else
		{
			stop = std::chrono::high_resolution_clock::now();
			isRunning = false;
			return true;
		}
	}

	bool GTimer2::Start()
	{
		if (isRunning)
		{
			return false;
		}
		else
		{
			start = std::chrono::high_resolution_clock::now();
			isRunning = true;
			return true;
		}
	}
}