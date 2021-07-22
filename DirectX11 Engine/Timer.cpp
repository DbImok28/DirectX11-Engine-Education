#include "Timer.hpp"

Timer::Timer()
{
	start = std::chrono::high_resolution_clock::now();
	stop = std::chrono::high_resolution_clock::now();
}

double Timer::GetMilisecondsElapsed()
{
	using namespace std::chrono;
	if (isRunning)
	{
		auto elapsed = std::chrono::duration<double, std::milli>(high_resolution_clock::now() - start);
		return elapsed.count();
	}
	else
	{
		auto elapsed = std::chrono::duration<double, std::milli>(stop - start);
		return elapsed.count();
	}
}

void Timer::Restart()
{
	isRunning = true;
	start = std::chrono::high_resolution_clock::now();
}

bool Timer::Stop()
{
	if (!isRunning)
		return false;
	else
	{
		isRunning = false;
		stop = std::chrono::high_resolution_clock::now();
		return true;
	}
}

bool Timer::Start()
{
	if (isRunning)
		return false;
	else
	{
		start = std::chrono::high_resolution_clock::now();
		isRunning = true;
		return true;
	}
}
