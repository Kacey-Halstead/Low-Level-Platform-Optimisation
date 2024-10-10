#pragma once
#include <chrono>
using namespace std::chrono;

namespace Timer
{


	float GetElapsedTime();
	
	void StartTimer();

	void EndTimer();
};

