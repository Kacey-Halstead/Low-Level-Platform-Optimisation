#pragma once
#include <vector>
#include <map>

class MemoryPool
{
public:
	MemoryPool(int numChunks, size_t eachChunkSize);
	~MemoryPool();

	void* Alloc(size_t iSize);
	bool Free(void* p);

	bool IsFull();
	int GetMemUsed();

	size_t chunkSize;
	size_t poolSize;
	int numberOfChunks;

private:
	void* StartOfPoolPtr;
	void** freeMemoryArr;
	int freeBlocks;
};

