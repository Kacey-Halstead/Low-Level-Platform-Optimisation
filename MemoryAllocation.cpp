#include "MemoryAllocation.h"

struct Header
{
	int size; //size of main allocated section
	Types tracker;
	int checkValue;
	Header* previous;
};

struct Footer
{
	int reserved;
	Header* next;
};

Header* previousHeader = nullptr;

void* operator new(size_t size)
{
	return ::operator new(size, DEFAULT);
}

void* operator new (size_t size, Types pTracker)
{
	size_t nRequestedBytes = size + sizeof(Header) + sizeof(Footer); //total = requested size + header + footer
	char* pMem = (char*)malloc(nRequestedBytes); //allocate
	Header* pHeader = (Header*)pMem; //set header pointer to start of allocated mem

	pHeader->size = size;
	pHeader->tracker = pTracker;

	void* pFooterAddr = pMem + sizeof(Header) + size; //pointer to footer
	Footer* pFooter = (Footer*)pFooterAddr; 

	if (previousHeader != nullptr) //if there is a previous header
	{
		void* footerLoc = previousHeader + sizeof(previousHeader) + previousHeader->size;
		Footer* prevFooter = (Footer*)footerLoc; //get footer location

		prevFooter->next = pHeader; //set next header in prev footer to this one
		pHeader->previous = previousHeader; //set previous header in this header to previousHeader
	}

	previousHeader = pHeader; //Set this header to new previous header
		
	switch (pTracker)
	{
	case DEFAULT:
		Tracker::AddBytesAllocated(size);
		break;

	case CUBE:
		CubeTracker::AddBytesAllocated(size);
		break;

	case SPHERE:
		SphereTracker::AddBytesAllocated(size);
		break;
	default:
		break;
	}


	return pMem + sizeof(Header);
}

void operator delete (void * pMem)
{
	Header* pHeader = (Header*)((char*)pMem - sizeof(Header));
	Footer* pFooter = (Footer*)((char*)pMem + pHeader->size);

	size_t toRemove = sizeof(&pHeader) + sizeof(&pMem) + sizeof(&pFooter);


	Header* prev = pHeader->previous;
	Header* next = pFooter->next;

	void* footerLoc = prev + sizeof(prev) + prev->size; 
	Footer* prevFooter = (Footer*)footerLoc; //previous footer

	prevFooter->next = next;
	next->previous = prev;

	switch (pHeader->tracker)
	{
	case DEFAULT:
		Tracker::RemoveBytesAllocated(toRemove);
		break;

	case CUBE:
		CubeTracker::RemoveBytesAllocated(toRemove);
		break;

	case SPHERE:
		SphereTracker::RemoveBytesAllocated(toRemove);
		break;
	default:
		break;
	}

	free(pHeader);
}