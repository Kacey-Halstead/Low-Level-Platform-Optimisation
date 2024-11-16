#pragma once
#include <Thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

namespace ManageThreads
{
	void Destroy();

	void Enqueue(std::function<void()> task);

	void WaitForThreadsNotBusy();

	void Init(int numThreads);
};