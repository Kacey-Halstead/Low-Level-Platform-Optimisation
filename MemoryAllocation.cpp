#include "MemoryAllocation.h"
#include "MemoryPool.h"
#include <array>

struct Header
{
	int size; //size of main allocated section
	Types tracker;
	Header* previous;
	Header* next;
	int checkValueH;
};

struct Footer
{
	int checkValueF;
};

namespace MemoryAlloc
{
	MemoryPool* GetPool(int index)
	{
		static std::array<MemoryPool, 3> pools = { MemoryPool(1000, 200), MemoryPool(750, 150), MemoryPool( 500,100) };
		return &pools[index];
	}

	Header* lastHeader = nullptr;
	Header* firstHeader = nullptr;

	const int headerCheckValue = 0xDEADC0DE;
	const int footerCheckValue = 0xDEADBEEF;

	void WalkTheHeap()
	{
		Header* header = firstHeader;
		int counter = 0;
		int totalMemCount = 0;

		while (header != nullptr)
		{
			Footer* footer = (Footer*)((char*)header + sizeof(Header) + header->size);

			totalMemCount += header->size;

			std::cout << std::endl << ++counter << ". Size: " << header->size;
			std::cout << " | Tracker Type: " << header->tracker;

			//check values
			if (footer->checkValueF != MemoryAlloc::footerCheckValue)
			{
				std::cout << " | Footer buffer overflow at :" << footer;
			}

			if (header->checkValueH != MemoryAlloc::headerCheckValue)
			{
				std::cout << " | Header buffer overflow at :" << header;
				break;
			}
			std::cout << std::endl;

			header = header->next; //set header to next header in list

		}
		std::cout << "\nTotal memory used on heap: " << totalMemCount << std::endl;
	}
};


void* operator new(size_t size)
{
	return ::operator new(size, DEFAULT);
}

MemoryPool* GetPoolSize(size_t size)
{
	MemoryPool* biggestPool = MemoryAlloc::GetPool(0);
	MemoryPool* mediumPool = MemoryAlloc::GetPool(1);
	MemoryPool* smallPool = MemoryAlloc::GetPool(2);

	if (size <= smallPool->chunkSize)
	{
		return smallPool;
	}
	else if(size > smallPool->chunkSize && size <= mediumPool->chunkSize)
	{
		return mediumPool;
	}
	else
	{
		return biggestPool;
	}
}

void* operator new (size_t size, Types pTracker)
{
	size_t nRequestedBytes = size + sizeof(Header) + sizeof(Footer); //total = requested size + header + footer

	char* pMem = (char*)GetPoolSize(nRequestedBytes)->Alloc(nRequestedBytes);
	if(pMem == nullptr) //if no room in memory pool
	{
		pMem = (char*)malloc(nRequestedBytes); //allocate
	}

	Header* pHeader = (Header*)pMem; //set header pointer to start of allocated mem
	pHeader->size = size;
	pHeader->tracker = pTracker;

	Footer* pFooter = (Footer*)(pMem + sizeof(Header) + size); //pointer to footer

	pHeader->checkValueH = MemoryAlloc::headerCheckValue;
	pFooter->checkValueF = MemoryAlloc::footerCheckValue;

	if (MemoryAlloc::lastHeader != nullptr) //if there is a previous header
	{
		MemoryAlloc::lastHeader->next = pHeader; //set next header in prev footer to this one
		pHeader->next = nullptr;
		pHeader->previous = MemoryAlloc::lastHeader; //set previous header in this header to previousHeader
	}
	else
	{
		MemoryAlloc::firstHeader = pHeader;
	}

	MemoryAlloc::lastHeader = pHeader; //Set this header to new previous header
		
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

	if (pHeader->checkValueH != MemoryAlloc::headerCheckValue)
	{
		std::cout << "Header buffer overflow at :" << pHeader << std::endl;
	}

	if (pFooter->checkValueF != MemoryAlloc::footerCheckValue)
	{
		std::cout << "Footer buffer overflow at :" << pFooter << std::endl;
	}

	Header* prev = pHeader->previous;
	Header* next = pHeader->next;

	if (next != nullptr) //if next exists
	{
		next->previous = prev;
	}

	if (prev != nullptr) //if previous exists
	{
		prev->next = next;
	}

	if (MemoryAlloc::firstHeader == pHeader) //if current is first header in list
	{
		MemoryAlloc::firstHeader = next;
	}

	if (MemoryAlloc::lastHeader == pHeader) //if current is last header in list
	{
		MemoryAlloc::lastHeader = prev;
	}

	switch (pHeader->tracker)
	{
	case DEFAULT:
		Tracker::RemoveBytesAllocated(pHeader->size);
		break;

	case CUBE:
		CubeTracker::RemoveBytesAllocated(pHeader->size);
		break;

	case SPHERE:
		SphereTracker::RemoveBytesAllocated(pHeader->size);
		break;
	default:
		break;
	}

	int size = sizeof(Header) + pHeader->size + sizeof(Footer);
	MemoryPool* pool = GetPoolSize(size);

	if (!pool->Free(pHeader)) //if not work
	{
		free(pHeader);
	}
}