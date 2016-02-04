#pragma once

#include "Allocator.h"
#include <stdlib.h>

struct Page // A page contains a constant amount of same sized memory slots
{
	size_t slotSize;
	unsigned slotCount, slotsLeft;
	void *nextSlot;
	Page *nextPage;
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
		Page *page = NULL;
		if (page->slotsLeft == 0)
		{
			// No room in current page, check if there's an another page with same sized slots and free space
			Page *anotherPage = findPageWithRoom(page);
			if (anotherPage == NULL)
			{
				// If page was not found, create a new page
				Page *newPage = createNewPage(size);
				page->nextPage = newPage;
				page = newPage;
			}
			else
			{
				page = anotherPage;
			}
		}
	}

	void deallocate(void* data) 
	{

	}

	Page *findPageWithRoom(Page *page)
	{

	}

	Page *createNewPage(size_t size)
	{
		// Creates a new page and reserves just enough space for all the blocks to fit
		Page *page = (Page*) malloc(sizeof(Page) + size * poolSize);
		page->slotSize = size;
		page->slotCount = poolSize;
		page->slotsLeft = poolSize;
		page->nextSlot = page + 1;
		page->nextPage = NULL;
	}

private:
	size_t objectSize;
	static const unsigned int poolSize = 100; // Max. number of items in one page
};