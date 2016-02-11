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

	// Allocate memory
	void *PagePoolAllocator::allocate(size_t size)
	{
		PageMap::iterator p = pageMap.find(size);

		PageHeader *page = NULL;

		if (p == pageMap.end())
		{
			// Creates new pageheader
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
			// Empty slot is used
			Slot *slot = (Slot*)page->nextSlot;

			pointer = page->nextSlot;

			page->nextSlot = slot->data;

			--page->freeSpaceCount;
		}

		--page->slotsLeft;

		return pointer;
	}
	
	// Deallocate used memory
	void PagePoolAllocator::deallocate(void *data)
	{
		PageHeader *page = NULL;

		//Checks if there are memory used in pages which can be deallocated
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
		//data = reinterpret_cast<void*>(value);

		page->nextSlot = data;

		++page->freeSpaceCount;
		++page->slotsLeft;

		
	}

	// Checks if there are pages with free room
	PageHeader *PagePoolAllocator::findPageWithRoom(PageHeader *page)
	{
		if (page->slotsLeft > 0)
		{
			return page;
		}

		if (page->nextPage == NULL)
		{
			return NULL;
		}

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

		// Holds information of the pages, aka book.
		HeaderLocationInfo headerLocationInfo;
		headerLocationInfo.page = page;
		headerLocationInfo.bottom = page + 1;
		headerLocationInfo.top = (char*)headerLocationInfo.bottom + page->slotCount * page->slotSize;

		headerLocations.push_back(headerLocationInfo);

		return page;
	}
	
	void* PagePoolAllocator::operator new(size_t size)
	{
		return allocator.allocate(size);
	}

		void PagePoolAllocator::operator delete(void* ptr)
	{
		allocator.deallocate(ptr);
	}
}