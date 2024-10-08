#include "Box.h"
#include "MemoryAllocation.h"

void* Box::operator new(size_t size)
{
	return ::operator new(size, CUBE);
}