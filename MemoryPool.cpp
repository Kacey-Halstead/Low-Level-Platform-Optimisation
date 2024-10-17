#include "MemoryPool.h"

MemoryPool::MemoryPool(int numChunks, size_t eachChunkSize)
{
	poolSize = numChunks*eachChunkSize;
	chunkSize = eachChunkSize;
	numberOfChunks = numChunks;

	StartOfPoolPtr = (char*)malloc(poolSize); //creates big chunk of memory
	if (StartOfPoolPtr == nullptr) return;

	freeMemoryArr = (void**)malloc(sizeof(void*) * numberOfChunks); //creates array of pointers to the pointers to memory and is the size of numberofchunks
	if (freeMemoryArr == nullptr) return;
	freeBlocks = numberOfChunks;

	for (int i = 0; i < numberOfChunks; i++)
	{
		void* point = (char*)StartOfPoolPtr + (chunkSize * i);
		freeMemoryArr[i] = point;
	}
}

MemoryPool::~MemoryPool()
{
	free(StartOfPoolPtr);
}

void* MemoryPool::Alloc(size_t iSize)
{
	if (freeBlocks > 0) //are any blocks free?
	{
		void* memPtr = freeMemoryArr[freeBlocks - 1]; //set pointer to void (take out of free list)
		freeBlocks--;
		return memPtr;
	}

	return nullptr;
}

bool MemoryPool::Free(void* p)
{
	if (StartOfPoolPtr <= p && p < ((char*)StartOfPoolPtr + poolSize)) //is pointer in pool?
	{
		freeMemoryArr[freeBlocks] = p; //add to free blocks
		freeBlocks++;
		return true;
	}

	return false;
}

bool MemoryPool::IsFull()
{
	return !freeBlocks;
}

int MemoryPool::GetMemUsed()
{
	return (numberOfChunks - freeBlocks) * chunkSize;
}
