#pragma once
#include <stdlib.h>
#include <iostream>
#include "Tracker.h"
#include "MemoryPool.h"

namespace MemoryAlloc
{
	MemoryPool* GetPool(int index);
	void WalkTheHeap();

};


void* operator new (size_t size);
void* operator new (size_t size, Types pTracker);
void operator delete(void* pMem);
