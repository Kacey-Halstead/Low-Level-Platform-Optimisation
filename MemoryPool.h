#pragma once
#include <vector>
#include <map>


class MemoryPool
{
public:
	MemoryPool(size_t iObjectSize, size_t eachChunkSize);
	~MemoryPool();

	std::map<char*, bool> pointers;
	size_t poolSize;
	size_t chunkSize;

	void* Alloc(size_t iSize);
	void Free(void* p, size_t iSize);
};

