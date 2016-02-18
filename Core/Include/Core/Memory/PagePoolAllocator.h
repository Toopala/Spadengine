#pragma once

#include <stdlib.h>
#include "Core/Assert.h"
#include "Core/Types.h"
#include <vector>
#include <map>
#include <iostream>

namespace sge
{
	struct PageHeader // A page contains a constant amount of same sized memory slots
	{
		size_t slotSize;
		unsigned slotCount, slotsLeft;
		void *nextSlot;

		unsigned freeSpaceCount;	// Check if the pages have free space

		PageHeader *nextPage;
	};

	struct HeaderLocationInfo	// 
	{
		PageHeader *page;
		void *top;
		void *bottom;
	};

	struct Slot
	{
		void *data;
	};

	class PagePoolAllocator
	{
	public:
		PagePoolAllocator();

		~PagePoolAllocator();

		typedef std::map<size_t, PageHeader*> PageMap;

		static const unsigned int poolSize = 100; // Max. number of items in one page
		
		void* allocate(size_t size);

		void deallocate(void* data);

		template <typename T, typename... Args>
		T* create(Args... args)
		{
			T *obj = (T*)allocate(sizeof(T*));
			new (obj)T(args...);

			return obj;
		}

		template <typename T>
		void destroy(T* ptr)
		{
			ptr->~T();
			deallocate(ptr);
		}

	private:
		PageHeader *createNewPageHeader(size_t size);			// Creating a new page
		PageHeader *findPageWithRoom(PageHeader *page);			// Check if there are same size of block in a different page
		PageMap pageMap;
		std::vector<HeaderLocationInfo> headerLocations;
	};
	extern PagePoolAllocator allocator;
}