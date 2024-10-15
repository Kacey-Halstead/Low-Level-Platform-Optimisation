#include "Timer.h"
#include <iostream>

namespace Timer
{
	duration<float, std::milli> elapsedTime;
	time_point<steady_clock> startPoint;
	time_point<steady_clock> endPoint;

	float GetElapsedTime()
	{
		return elapsedTime.count();
	}

	void StartTimer()
	{
		startPoint = steady_clock::now();
	}

	void EndTimer()
	{
		endPoint = steady_clock::now();
		elapsedTime = endPoint - startPoint;
		std::cout << "\nTimer: " << Timer::GetElapsedTime() << std::endl;
	}
}

