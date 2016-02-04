#include "Core\Memory\PagePoolAllocator.h"

namespace sge
{
	PagePoolAllocator::PagePoolAllocator()
	{

	}
	PagePoolAllocator::~PagePoolAllocator()
	{

	}

	void *PagePoolAllocator::allocate(size_t size)
	{
		PageHeader *page = NULL;
		if (page->slotsLeft == 0)
		{
			// No room in current page, check if there's an another page with same sized slots and free space
			PageHeader *anotherPage = findPageWithRoom(page);
			if (anotherPage == NULL)
			{
				// If page was not found, create a new page
				PageHeader *newPage = createNewPageHeader(size);
				page->nextPage = newPage;
				page = newPage;
			}
			else
			{
				page = anotherPage;
			}
		}

		void *pointer = NULL;

		if (page->freeSpaceCount <= 0)
		{
			pointer = page->nextSlot;
			page->nextSlot = (char*)page->nextSlot + size;
		}
		else
		{
			// K00PA HELP US

			//char* p = (char*)page->nextSlot;
			//
			//// Empty slot is used next
			//Slot *slot = (Slot*)page->nextSlot;

			////... 
			//pointer = page->nextSlot;

			//page->nextSlot = slot->data;

			//--page->freeSpaceCount;
		}

		return pointer;
	}
	
	void PagePoolAllocator::deallocate(void *data)
	{
	
	}

	PageHeader *PagePoolAllocator::findPageWithRoom(PageHeader *header)
	{
		return nullptr;
	}
	PageHeader *PagePoolAllocator::createNewPageHeader(size_t size)
	{
		// Creates a new page and reserves just enough space for all the blocks to fit
		PageHeader *page = (PageHeader*)malloc(sizeof(PageHeader) + size * poolSize);
		page->slotSize = size;
		page->slotCount = poolSize;
		page->slotsLeft = poolSize;
		page->nextSlot = page + 1;
		page->freeSpaceCount = 0;
		page->nextPage = NULL;

		// Holds information of the pages, aka book.
		HeaderLocationInfo headerLocationInfo;
		headerLocationInfo.page = page;
		headerLocationInfo.bottom = page + 1;
		headerLocationInfo.top = (char*)headerLocationInfo.bottom + page->slotCount * page->slotSize;

		headerLocations.push_back(headerLocationInfo);

		return page;
	}
}