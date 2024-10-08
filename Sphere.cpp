#include "Sphere.h"
#include "MemoryAllocation.h"

void* Sphere::operator new(size_t size)
{
	return ::operator new(size, SPHERE);
}