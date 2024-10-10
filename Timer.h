#pragma once
#include <chrono>
using namespace std::chrono;

namespace Timer
{
	duration<float, std::milli> elapsedTime;
	time_point<steady_clock> startPoint;
	time_point<steady_clock> endPoint;

	duration<float, std::milli> GetElapsedTime();
	
	void StartTimer();

	void EndTimer();
};

