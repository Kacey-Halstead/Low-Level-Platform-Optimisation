#include "MemoryPool.h"

MemoryPool::MemoryPool(size_t iObjectSize, size_t eachChunkSize)
{
	poolSize = iObjectSize;
	chunkSize = eachChunkSize;

	pMem = (char*)malloc(iObjectSize);

	int numChunks = poolSize / chunkSize;
	for (int i = 0; i < numChunks; i++)
	{
		void* point = (char*)pMem + (chunkSize * i);
		pointers[point] = true; //set all to free
	}
}

MemoryPool::~MemoryPool()
{
	free(pMem);
}

void* MemoryPool::Alloc(size_t iSize)
{
	for (auto& p : pointers) //check for free memory
	{
		if (p.second && iSize <= chunkSize) //if memory free and right size, return pointer to memory
		{
			p.second = false;
			return p.first;
		}
	}

	return nullptr;
}

void MemoryPool::Free(void* p)
{
	auto it = pointers.find(p);
	if (it == pointers.end())
	{
		// need to call default free in delete operator
	}
	else
	{
		it->second = true;
	}
}