#pragma once
#include <stdlib.h>
#include <iostream>

void* operator new (size_t size);
void operator delete(void* pMem);
