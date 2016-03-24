#pragma once

#include <stdlib.h>
#include "Core/Assert.h"
#include "Core/Types.h"
#include <vector>
#include <map>
#include <iostream>

namespace sge
{
	/** \brief PageHeader struct.
	*
	*	A page contains a constant amount of same sized memory slots.
	*
	*	\param size_t slotSize : Size of a single memory slot.
	*	\param unsigned slotCount : Number of memory slots in one page.
	*	\param unsigned slotsLeft : Number of memory slots left in one page.
	*	\param unsigned freeSpaceCount : Keeps count on the slots that have been removed from the middle.
	*	\param void *nextSlot : Pointer to the next slot.
	*	\param PageHeader *nextPage : Pointer to the nextPage.
	*/
	struct PageHeader
	{
		size_t slotSize;
		unsigned slotCount, slotsLeft, freeSpaceCount;
		void *nextSlot;
		PageHeader *nextPage;
	};

	/** \brief HeaderLocationInfo struct.
	*
	*	Information about the "book", where all the pages are stored.
	
	*	\param PageHeader *page : Pointer to page.
	*	\param void *top : Pointer to the top of the "book".
	*	\param void *bottom : Pointer to the bottom of the "book".
	*/
	struct HeaderLocationInfo
	{
		PageHeader *page;
		void *top, *bottom;
	};

	/** \brief Slot struct.
	*
	*	Pointer to empty slot, if the page has empty slots left.
	*	\param void *data : Pointer to slot.
	*/
	struct Slot
	{
		void *data;
	};

	/** \brief Memory allocator.
	*	Memory allocator that uses PagePoolAllocator style.
	*
	*	
	*/
	class PagePoolAllocator
	{
	public:
		/** \brief A constructor.
		*/
		PagePoolAllocator();

		/** \brief A destructor.
		*/
		~PagePoolAllocator();

		/** \brief PageMap.
		*
		*	\param typedef std::map<size_t, PageHeader*> PageMap : Aka. book, that holds all the pages together.
		*/
		typedef std::map<size_t, PageHeader*> PageMap;

		/** \brief poolSize
		*
		*	\param static const unsigned int poolSize : Max. number of items in one page
		*/
		static const unsigned int poolSize = 100; 
		
		/** \brief Allocate
		*
		*	Allocates memory in pages. Checks if the pages has the same type of memory slots free.
		*	\param size_t size : Size of the allocated class.
		*	\return pointer : Returns pointer to the allocated slot.
		*/
		void* allocate(size_t size);

		/** \brief Deallocate
		*
		*	Checks the pages, if there are same type of memory used and which can be deallocated.
		*	\param void* data : Pointer to slots which we want to deallocate.
		*/
		void deallocate(void* data);

		/** \brief A function that is used instead of new.
		*
		*	Calls allocate function in order to find an empty slot
		*	that is then used in conjunction with the new function.
		*	\param Args... args : Takes variable amount of class arguments.
		*	\return obj : Returns the pointer to the slot that's been allocated.
		*/
		template <typename T, typename... Args>
		T* create(Args... args)
		{
			T *obj = (T*)allocate(sizeof(T*));
			new (obj)T(args...);

			return obj;
		}

		/** \brief A function that marks the slot as free.
		*
		*	Calls the destructor of the object behind the pointer and calls deallocate function that marks the slot as free. 
		*	\param T* ptr : Pointer to the object that the user wants to destroy.
		*/
		template <typename T>
		void destroy(T* ptr)
		{
			ptr->~T();
			deallocate(ptr);
		}

	private:
		/** \brief Creates a new page header
		*
		*	Creates a new page header if the same memory type page is full or if the memory type is different than the last other pages.
		*	\param size_t size : Memory type size.
		*	\return page : Returns page.
		*/
		PageHeader *createNewPageHeader(size_t size);

		/** \brief Find page with room
		*
		*	Check if there are same size of slot in a different page.
		*	\param PageHeader *page : Pointer to page.
		*	\return findPageWithRoom(page->nextPage) : Returns page with same type of memory room left.
		*/
		PageHeader *findPageWithRoom(PageHeader *page);
		
		PageMap pageMap;

		/** \brief Vector for storing information about the "book".
		*/
		std::vector<HeaderLocationInfo> headerLocations;
	};
	extern PagePoolAllocator allocator;
}