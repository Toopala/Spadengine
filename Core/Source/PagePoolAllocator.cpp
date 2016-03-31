#include "Core/Memory/PagePoolAllocator.h"

namespace sge
{
	PagePoolAllocator allocator;

	PagePoolAllocator::PagePoolAllocator()
	{

	}

	PagePoolAllocator::~PagePoolAllocator()
	{
		
	}

	void *PagePoolAllocator::allocate(size_t size)
	{
		PageMap::iterator p = pageMap.find(size);
		PageHeader *page = NULL;

		if (p == pageMap.end())
		{
			page = createNewPageHeader(size);
			pageMap.insert(PageMap::value_type(size, page));
		}
		else
		{
			page = p->second;
		}

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
			Slot *slot = (Slot*)page->nextSlot;
			pointer = page->nextSlot;
			page->nextSlot = slot->data;
			--page->freeSpaceCount;
		}

		--page->slotsLeft;
		return pointer;
	}
	
	void PagePoolAllocator::deallocate(void *data)
	{
		PageHeader *page = NULL;

		// Finds where the pointer is located
		for (unsigned i = 0; headerLocations.size(); i++)
		{
			if (data >= headerLocations[i].bottom)
			{
				if (data < headerLocations[i].top)
				{
					page = headerLocations[i].page;
					break;
				}
			}
		}
		
		SGE_ASSERT(page);
		void* value = (void*)page->nextSlot;
		*(void**)data = value;
		page->nextSlot = data;
		++page->freeSpaceCount;
		++page->slotsLeft;	
	}

	PageHeader *PagePoolAllocator::findPageWithRoom(PageHeader *page)
	{
		// If there's room in current page, return the current page
		if (page->slotsLeft > 0)
		{
			return page;
		}
		// If there's no next page, return null
		if (page->nextPage == NULL)
		{
			return NULL;
		}
		// Else, return the next page
		return findPageWithRoom(page->nextPage);
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

		// Stores all the needed info from the page to new headerLocationInfo which is then pushed to a vector
		HeaderLocationInfo headerLocationInfo;
		headerLocationInfo.page = page;
		headerLocationInfo.bottom = page + 1;
		headerLocationInfo.top = (char*)headerLocationInfo.bottom + page->slotCount * page->slotSize;
		headerLocations.push_back(headerLocationInfo);

		return page;
	}
}