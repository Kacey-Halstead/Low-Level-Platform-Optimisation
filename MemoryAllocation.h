#pragma once
#include <stdlib.h>
#include <iostream>
#include "Tracker.h"

void* operator new (size_t size);
void* operator new (size_t size, Types pTracker);
void operator delete(void* pMem);
