#include "MemoryAllocation.h"


void* operator new (size_t size)
{
	char* pMem = (char*)malloc(size);
	void* pStartMemBlock = pMem;
	return pStartMemBlock;
}

void operator delete (void * pMem)
{
	free(pMem);
}