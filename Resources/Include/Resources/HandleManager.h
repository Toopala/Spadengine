#pragma once
#include <vector>
#include <cassert>


// TO DO ADD FUNCTIONALITY FROM HANDLEMGR TO RESMGR CLASS
// THIS WILL BE DELETED

namespace sge
{
	template <typename DATA, typename HANDLE>
	class HandleManager
	{
	private:
		typedef std::vector<DATA> userVec;
		typedef std::vector<unsigned int> magicVec;
		typedef std::vector<unsigned int> freeVec;

		userVec userData;			// Data we're going to get to
		magicVec magicNumbers;		// Corresponding magic numbers
		freeVec freeSlots;			// Keeps track of free slots in the database

	public:
		HandleManager() {}
		~HandleManager() {}

		// Acquisition
		DATA* acquire(HANDLE& handle);
		void release(HANDLE handle);

		// Dereferencing
		DATA* dereference(HANDLE handle);
		const DATA* dereference(HANDLE handle) const;

		// Other query
		unsigned int getUsedHandleCount() const
		{
			return (magicNumbers.size() - freeSlots.size());
		}

		bool hasUsedHandles() const
		{
			// 0 is converted to false, others are converted to true
			return (!!getUsedHandleCount());
		}
	};


	template <typename DATA, typename HANDLE>
	DATA* HandleManager<DATA, HANDLE> ::acquire(HANDLE& handle)
	{
		// If the free list is empty, add a new one, otherwise use the first one found

		unsigned int index;
		if (freeSlots.empty())
		{
			index = freeSlots.back();
			handle.init(index);
			userData.push_back(DATA());
			magicNumbers.push_back(handle.getMagic());
		}
		else
		{
			index = freeSlots.back();
			handle.init(index);
			freeSlots.pop_back();
			magicNumbers[index] = handle.getMagic();
		}
		return (userData.begin() + index);
	}

	template <typename DATA, typename HANDLE>
	void HandleManager<DATA, HANDLE> ::release(HANDLE handle)
	{
		// Which one?
		unsigned int index = handle.getIndex();

		// Make sure it is valid
		assert(index < userData.size());
		assert(magicNumbers[index] == handle.getMagic());

		// OK remove it - tag as unused and add to free list
		magicNumbers[index] = 0;
		freeSlots.push_back(index);
	}

	template <typename DATA, typename HANDLE>
	inline DATA* HandleManager<DATA, HANDLE> ::dereference(HANDLE handle)
	{
		if (handle.isNull())
			return 0;

		// Check handle validity - This check can be removed for speed
		// IF you assume all handle references are always valid
		unsigned int index = handle.getIndex();
		if ((index >= userData.size()) || (magicNumbers[index] != handle.getMagic()))
		{
			// Not good! Invalid handle == client programming error
			assert();
			return 0;
		}

		return (userData.begin() + index);
	}

	template <typename DATA, typename HANDLE>
	inline const DATA* HandleManager<DATA, HANDLE> ::dereference(HANDLE handle) const
	{
		// This lazy cast is OK - non-const version does not modify anything
		typedef HandleManager<DATA, HANDLE> ThisType;
		return (const_cast<ThisType*> (this)->dereference(handle));
	}
}