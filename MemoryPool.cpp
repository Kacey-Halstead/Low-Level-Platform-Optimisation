#include "MemoryPool.h"

MemoryPool::MemoryPool(size_t iObjectSize, size_t eachChunkSize)
{
	poolSize = iObjectSize;
	chunkSize = eachChunkSize;

	char* pMem = (char*)malloc(iObjectSize);

	int numChunks = poolSize / chunkSize;
	pointers.emplace(numChunks);

	for (int i = 1; i < numChunks; i++)
	{
		char* point = (char*)(pMem + (chunkSize * i));
		pointers[point] = true; //set all to free
	}
}

MemoryPool::~MemoryPool()
{

}

void* MemoryPool::Alloc(size_t iSize)
{
	for (auto& p : pointers) //check for free memory
	{
		if (!p.second && iSize <= chunkSize) //if memory free and right size, return pointer to memory
		{
			p.second = false;
			return p.first;
		}
	}

	return nullptr;
}

void MemoryPool::Free(void* p, size_t iSize)
{
	free(p);
	pointers[(char*)p] = true;
}