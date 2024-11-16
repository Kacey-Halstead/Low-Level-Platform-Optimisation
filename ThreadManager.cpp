#include "ThreadManager.h"

namespace ManageThreads
{
	std::vector<std::thread> threadVec;
	std::queue<std::function<void()>> taskQueue;
	std::mutex threadMutex;
	std::condition_variable threadCV;
	std::condition_variable mainThreadCV;
	int taskCounter = 0;
	bool stop = false;

	void Destroy()
	{
		//stop all threads and join
		std::unique_lock<std::mutex> lock(threadMutex);
		stop = true;
		threadCV.notify_all(); //runs all threads but will stop as stop = true
		for (auto& thread : threadVec)
		{
			thread.join();
		}
	}

	void Enqueue(std::function<void()> task)
	{
		//assign task and notify a thread
		{
			std::unique_lock<std::mutex> lock(threadMutex);
			taskQueue.emplace(move(task));
		}
		threadCV.notify_one();
	}

	void WaitForThreadsNotBusy()
	{
		std::unique_lock<std::mutex> lock(threadMutex);

		mainThreadCV.wait(lock, [&] {
			return taskQueue.empty() && taskCounter == 0; //waits until all other threads are not busy
			});
	}

	void Init(int numThreads)
	{
		if (threadVec.empty())
		{
			for (size_t i = 0; i < numThreads; i++)
			{
				//every thread assigned while loop that waits for task and completes it
				threadVec.emplace_back([&] {
					while (true)
					{
						std::function<void()> task;
						{
							std::unique_lock<std::mutex> lock(threadMutex);
							threadCV.wait(lock, [&] {
								return !taskQueue.empty() || stop; //wait until task available or stop is true
								});

							if (stop && taskQueue.empty())
							{
								return;
							}

							task = move(taskQueue.front()); //set task
							taskQueue.pop();
							taskCounter++;

							lock.unlock();
							task();
							lock.lock();
							taskCounter--;
							mainThreadCV.notify_one(); //checks wait condition for main thread
						}
					}
					});
			}
		}
	}
}