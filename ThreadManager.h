#pragma once
#include <Thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

namespace ManageThreads
{
	std::vector<std::thread> threadVec;
	std::queue<std::function<void()>> threadTasks;
	std::mutex threadMutex;
	std::condition_variable threadCV;
	bool stop = false;

	void Destroy()
	{
		std::unique_lock<std::mutex> lock(threadMutex);
		stop = true;
		threadCV.notify_all();
		for (auto& thread : threadVec)
		{
			thread.join();
		}
	}

	void Enqueue(std::function<void()> task)
	{
		{
			std::unique_lock<std::mutex> lock(threadMutex);
			threadTasks.emplace(move(task));
		}
		threadCV.notify_one();
	}

	void Init(int numThreads)
	{
		if (threadVec.empty())
		{
			for (size_t i = 0; i < numThreads; i++)
			{
				threadVec.emplace_back([&] {
					while (true)
					{
						std::function<void()> task;
						{
							std::unique_lock<std::mutex> lock(threadMutex);
							threadCV.wait(lock, [&] {
								return !threadTasks.empty() || stop;
								});

							if (stop && threadTasks.empty())
							{
								return;
							}

							task = move(threadTasks.front());
							threadTasks.pop();
						}
						task();
					}
					});
			}
		}
	}
};