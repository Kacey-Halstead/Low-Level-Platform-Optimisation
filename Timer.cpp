#include "Timer.h"

void Timer::StartTimer()
{
	startPoint = steady_clock::now();
}

void Timer::EndTimer()
{
	endPoint = steady_clock::now();
	elapsedTime = endPoint - startPoint;
}