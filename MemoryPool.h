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

	size_t chunkSize;

private:
	Pair* pairArray;
	void* pMem;
	size_t poolSize;

	int numberOfChunks;
};

