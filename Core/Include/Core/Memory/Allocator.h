#pragma once
#include <assert.h>

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

	// 
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