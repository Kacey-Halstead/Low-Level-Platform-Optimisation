#pragma once
#include <stdlib.h>
#include <iostream>
#include "Tracker.h"

void* operator new (size_t size, Tracker* pTracker);
void operator delete(void* pMem, Tracker* pTracker);
