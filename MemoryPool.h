#pragma once
#include <vector>
#include <map>


class MemoryPool
{
public:
	MemoryPool(size_t iObjectSize, size_t eachChunkSize);
	~MemoryPool();

	void* Alloc(size_t iSize);
	void Free(void* p);

private:
	std::map<void*, bool> pointers; // Change map to malloc allocated array of pairs or indexes using ((p - pMem) / chunkSize)
	void* pMem;
	size_t poolSize;
	size_t chunkSize;
};

