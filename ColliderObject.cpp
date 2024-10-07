#include "ColliderObject.h"

void* ColliderObject::operator new(size_t size)
{
	return ::operator new(size);
}

void ColliderObject::operator delete(void* p, size_t size)
{
	::operator delete(p);
}