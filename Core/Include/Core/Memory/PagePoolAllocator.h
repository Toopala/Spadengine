#pragma once

#include <stdlib.h>
#include <vector>
#include <map>

#include "Core/Assert.h"
#include "Core/Types.h"

namespace sge
{
	/** \brief Contains a constant amount of same sized memory slots and keeps track on used and unused slots. */
	struct PageHeader
	{
		size_t slotSize;			/**<  Size of a single memory slot. */
		unsigned slotCount;			/**<  Number of memory slots in a page. */
		unsigned slotsLeft;			/**<  Number of memory slots left in a page. */
		unsigned freeSpaceCount;	/**<  Keeps count on the slots that have been pointing to something but is now deleted. */
		void *nextSlot;				/**<  Points to the slot that is going to be used next. */
		PageHeader *nextPage;		/**<  Points to the next page. */
	};

	/** \brief Keeps track of the information on allocated memory. */
	struct HeaderLocationInfo
	{
		PageHeader *page;	/**<  Points to a page. */
		void *bottom;		/**<  The first slot in the page. */
		void *top;			/**<  The last slot in the page. */
	};

	/** \brief A struct that is used to store a pointer. */
	struct Slot
	{
		void *data;	/**<  Pointer to slot data. */
	};

	/** \brief The class that manages memory.
	*
	*	The allocator uses PagePool style which means that the memory is divided by size.
	*	Objects of different sizes are on different pages to make it quick and easy to allocate and deallocate memory.
	*/
	class PagePoolAllocator
	{
	public:
		/** \brief The default constructor. */
		PagePoolAllocator();

		/** \brief The destructor. */
		~PagePoolAllocator();
		
		/** \brief Allocates memory in pages.
		*
		*	If there are no pages with room for the given object type, a new page is created and the object is then assigned to the first pointer.
		*
		*	\param size_t size : Size of the object.
		*	\return Returns pointer to the allocated slot.
		*/
		void* allocate(size_t size);

		/** \brief Deallocate memory from pages.
		*
		*	Finds the object behind the given pointer and marks it as unused.
		*
		*	\param void* data : Pointer to the data we want to get rid of.
		*/
		void deallocate(void* data);

		/** \brief A template function that is used instead of operator new.
		*
		*	Calls allocate function in order to find an empty slot and then creates the object to that slot.
		*
		*	\param Args... args : Takes variable amount of class arguments.
		*	\return Returns the pointer to the slot that has been allocated.
		*/
		template <typename T, typename... Args>
		T* create(Args... args)
		{
			T *obj = (T*)allocate(sizeof(T));
			new (obj)T(args...);

			return obj;
		}

		/** \brief A function that destroys an object.
		*
		*	Calls the destructor of the object behind the given pointer and then calls the deallocate function which marks the slot that was used as free.
		*	\param T* ptr : Pointer to the object that the user wants to destroy.
		*/
		template <typename T>
		void destroy(T* ptr)
		{
			ptr->~T();
			deallocate(ptr);
		}

		typedef std::map<size_t, PageHeader*> PageMap; /**<  A map that keeps track of the pages. You can think of it as the book that holds the pages. */
		static const unsigned int poolSize = 100; /**<  The amount of slots in a page. */

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
		
		/**<	The map that contains all the pages.
		*		\see PageMap
		*/
		PageMap pageMap;

		std::vector<HeaderLocationInfo> headerLocations; /**<  Vector for storing information about the pages. */
	};
	extern PagePoolAllocator allocator;
}