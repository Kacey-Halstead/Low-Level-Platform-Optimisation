#include "MemoryPool.h"

MemoryPool::MemoryPool(size_t iObjectSize, size_t eachChunkSize)
{
	poolSize = iObjectSize;
	chunkSize = eachChunkSize;
	numberOfChunks = poolSize / chunkSize;;

	pMem = (char*)malloc(iObjectSize);
	if (pMem == nullptr) return;

	pairArray = (Pair*)malloc(sizeof(Pair) * numberOfChunks);
	if (pairArray == nullptr) return;

	for (int i = 0; i < numberOfChunks; i++)
	{
		void* point = (char*)pMem + (chunkSize * i);
		pairArray[i] = Pair{ point, true };
	}
}

MemoryPool::~MemoryPool()
{
	free(pMem);
}

void* MemoryPool::Alloc(size_t iSize)
{
	for (int i = 0; i < numberOfChunks; i++) //check for free memory
	{

		if (pairArray[i].isFree && iSize <= chunkSize) //if memory free and right size, return pointer to memory
		{
			pairArray[i].isFree = false;
			return pairArray[i].memPtr;
		}
	}

	return nullptr;
}

bool MemoryPool::Free(void* p)
{
	for (int i = 0; i < numberOfChunks; i++)
	{
		if (pairArray[i].memPtr == p)
		{
			pairArray[i].isFree = true;
			return true;
		}
	}

	return false;
}