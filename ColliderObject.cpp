#include "ColliderObject.h"

void* ColliderObject::operator new(size_t size, Types* tTracker)
{
	return ::operator new(size, tTracker);
}

void ColliderObject::operator delete(void* p, size_t size)
{
	::operator delete(p);
}