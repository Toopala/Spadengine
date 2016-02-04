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

		return nullptr;
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
		page->nextPage = NULL;

		return nullptr;
	}
}