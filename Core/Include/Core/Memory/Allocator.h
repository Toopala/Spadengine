#pragma once

#include <assert.h>
#include "Core\Types.h"

namespace sge
{
	namespace pointer_math
	{
		void* add(void* p, size_t x);
		void* substract(void* p, size_t x);
	}

	class Allocator
	{
	public:
		Allocator(size_t s)
		{
			size = s;

			usedMemory = 0;
			numAllocations = 0;
		}
		virtual~Allocator()
		{
			assert(numAllocations == 0 && usedMemory == 0);

			size = 0;
		}


		virtual void* allocate(size_t size) = 0;

		virtual void deallocate(void* data) = 0;

		size_t getSize() const
		{
			return size;
		}

		size_t getUsedMemory() const
		{
			return usedMemory;
		}

		size_t getNumAllocations() const
		{
			return numAllocations;
		}


	protected:
		size_t size;
		size_t usedMemory;
		size_t numAllocations;

	};

	// Templates for allocating and deallocating
	namespace allocator
	{
		template <class T> T* allocateNew(Allocator& allocator)
		{
			return new (allocator.allocate(sizeof(T), __alignof(T))) T;
		}

		template <class T> T* deleteAllocate(Allocator& allocator, T& object)
		{
			object.~T();
			allocator.deallocate(&object);
		}
	}

	//Inline functions definitions
	namespace pointer_math
	{
		inline void* add(void* p, size_t x)
		{
			return (void*)(reinterpret_cast<uptr>(p)+x);
		}

		inline void* substract(void* p, size_t x)
		{
			return (void*)(reinterpret_cast<uptr>(p)-x);
		}
	}
}
