#pragma once
#include <vector>
#include <map>

struct Pair
{
	void* memPtr;
	bool isFree;
};

class MemoryPool
{
public:
	MemoryPool(size_t iObjectSize, size_t eachChunkSize);
	~MemoryPool();

	void* Alloc(size_t iSize);
	bool Free(void* p);

	bool isFull();


	size_t chunkSize;
	int memUsed = 0;
	size_t poolSize;

private:
	Pair* pairArray;
	void* pMem;

	int numberOfChunks;
};

