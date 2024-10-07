#pragma once
#include <chrono>
using namespace std::chrono;

class Timer
{
	duration<float, std::milli> elapsedTime;
	time_point<steady_clock> startPoint;
	time_point<steady_clock> endPoint;
	
	void StartTimer();

	void EndTimer();
};

