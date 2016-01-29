#pragma once

#include "Allocator.h"

struct Page // A page contains a constant amount of same sized memory slots
{
	size_t slotSize;
	unsigned slotCount, slotsLeft;
	void *nextSlot;
	Page *nextPageHeader;
};

class PagePoolAllocator : public sge::Allocator
{
public:
	PagePoolAllocator(size_t objSize, size_t s) : Allocator(s)
	{

	}

	~PagePoolAllocator() 
	{

	}

	void* allocate(size_t size)
	{
		
	}

	void deallocate(void* data) 
	{

	}

private:
	size_t objectSize;
	static const unsigned int poolSize = 100; // max items on one page
};