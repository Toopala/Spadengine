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

		template <typename T>
		T* create()
		{
			T* obj = (T*)allocate(sizeof(T*));
			new (obj)T();

			return obj;
		}

		// TODO: find a way to make 1 method than can check
		//the number of variables in constructor

		template <typename T, typename P1>
		T* create(P1 p1)
		{
			T *obj = (T*)allocate(sizeof(T*));
			new (obj)T(p1);

			return obj;
		}
		
		template <typename T, typename P1, typename P2>
		T* create(P1 p1, P2 p2)
		{
			T *obj = (T*)allocate(sizeof(T*));
			new (obj)T(p1, p2);

			return obj;
		}

		template <typename T, typename P1, typename P2, typename P3>
		T* create(P1 p1, P2 p2, P3 p3)
		{
			T *obj = (T*)allocate(sizeof(T*));
			new (obj)T(p1, p2, p3);

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