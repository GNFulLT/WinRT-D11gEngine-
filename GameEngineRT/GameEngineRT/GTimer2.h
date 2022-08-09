#pragma once
#include <chrono>
namespace GNF::Common {
	class GTimer2
	{
		public:
			GTimer2();
			double GetMilisecondsElapsed();
			void Restart();
			bool Stop();
			bool Start();
		private:
			bool isRunning = false;
			std::chrono::time_point<std::chrono::steady_clock> start;
			std::chrono::time_point<std::chrono::steady_clock> stop;
	};
}
