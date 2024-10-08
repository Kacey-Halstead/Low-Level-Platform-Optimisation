#include "MemoryAllocation.h"

struct Header
{
	int size; //size of main allocated section
	Tracker* tracker;
};

struct Footer
{
	int reserved;
};

void* operator new (size_t size, Tracker* pTracker)
{
	if (pTracker == nullptr)
	{
		Tracker* t = (Tracker*)malloc(sizeof(Tracker));
		Tracker* pTracker = (Tracker*)t;
	}

	size_t nRequestedBytes = size + sizeof(Header) + sizeof(Footer); //total = requested size + header + footer
	char* pMem = (char*)malloc(nRequestedBytes); //allocate
	Header* pHeader = (Header*)pMem; //set header pointer to start of allocated mem

	pHeader->size = size;
	pHeader->tracker = pTracker;

	void* pFooterAddr = pMem + sizeof(Header) + size; //pointer to footer
	Footer* pFooter = (Footer*)pFooterAddr; // cast footer to void pointer
		
	pTracker->AddBytesAllocated(size);

	return pMem + sizeof(Header);
}

void operator delete (void * pMem)
{
	Header* pHeader = (Header*)((char*)pMem - sizeof(Header));
	Footer* pFooter = (Footer*)((char*)pMem + pHeader->size);

	pHeader->tracker->RemoveBytesAllocated(sizeof(&pHeader) + sizeof(&pMem) + sizeof(&pFooter));

	free(pHeader);
}