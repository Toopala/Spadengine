#pragma once
#include <vector>
#include <cassert>
#include <unordered_map>
#include "Resources/Resource.h"
#include "Resources/TextureResource.h"

namespace sge
{
	template<class T>
	class Handle;
	// ----------------------------------------------------
	// Retrieving resource

	class ResourceManager
	{
	public:

		ResourceManager();
		~ResourceManager();

		template <class T>
		Handle<T> load(const std::string &filename)
		{
			if (filename.empty())
			{
				std::cout << "Filename cannot be empty! Error loading resource." << std::endl;
				return Handle<T>();
			}

			Handle<T> handle(this);
			unsigned int index;

			T* resource = new T(filename);

			// TODO HANDLE GET RESOURCE

			if (freeSlots.empty())
			{
				index = magicNumbers.size();
				handle.init(index);
				userData.insert({ filename, resource });
				magicNumbers.push_back(handle.getMagic());
			}
			else
			{
				index = freeSlots.back();
				handle.init(index);
				freeSlots.pop_back();
				magicNumbers[index] = handle.getMagic();
			}

			pathVec.push_back(filename);

			assert(pathVec.at(index) == filename);

			std::unordered_map<std::string, sge::Resource*>::iterator it;
			it = userData.find(filename);

			if (it != userData.end())
			{
				(*it).second->increaseRef();
			}

			return handle;
		};


		template <class T>
		void release(Handle<T> handle)
		{
			unsigned int index = handle.getIndex();

			//assert(index < pathVec.size());
			assert(magicNumbers[index] == handle.getMagic());

			magicNumbers[index] = 0;
			freeSlots.push_back(index);

			std::unordered_map<std::string, sge::Resource*>::iterator it;
			std::string filename = pathVec.at(index);
			it = userData.find(filename);

			if (it != userData.end())
			{
				(*it).second->decreaseRef();
			}

			std::cout << "Handle was released." << std::endl;
		};

		template <typename T>
		T* getResource(sge::Handle<T>& handle)
		{
			std::string resPath = pathVec.at(handle.getIndex());

			std::unordered_map<std::string, sge::Resource*>::iterator it;
			it = userData.find(resPath);
			return static_cast<T*>((*it).second);
		}


		bool unload(const std::string &filename);
		void printResources();

	private:

		std::unordered_map<std::string, sge::Resource*> userData;
		std::vector<unsigned int> magicNumbers;
		std::vector<unsigned int> freeSlots;
		std::vector<std::string> pathVec;

		void releaseAll();

	};

	template <typename TAG>
	class Handle
	{

		union
		{
			enum BITFIELD
			{
				// Sizes to use for bitfields
				MAX_BITS_INDEX = 16,
				MAX_BITS_MAGIC = 16,

				// Size to compare against for asserting deferences
				MAX_INDEX = (1 << MAX_BITS_INDEX) - 1,
				MAX_MAGIC = (1 << MAX_BITS_MAGIC) - 1,
			};

			struct
			{
				unsigned m_Index : MAX_BITS_INDEX;	// Index into resource array
				unsigned m_Magic : MAX_BITS_MAGIC;	// Magic number to check
			}MAXBITS;

			unsigned int m_Handle;
		}HANDLE;

		ResourceManager* refManager;
	public:
		template <typename T>
		T* getResource() {
			return static_cast<T*>(refManager->getResource<T>(*this));
		}
		// Creation sets the handle to null for error check purposes.
		// Handle needs to be initialized before it can be properly used.
		Handle() : refManager(nullptr) { HANDLE.m_Handle = 0; }
		Handle(ResourceManager* refManager) : refManager(refManager) { HANDLE.m_Handle = 0; }
		void init(unsigned int index);

		// Methods for managing our Handle
		unsigned int getIndex()		const { return (HANDLE.MAXBITS.m_Index); }
		unsigned int getMagic()		const { return (HANDLE.MAXBITS.m_Magic); }
		unsigned int getHandle()	const { return (HANDLE.m_Handle); }
		bool isNull()				const { return (!HANDLE.m_Handle); }

		operator unsigned int()		const { return(HANDLE.m_Handle); }
	};

	template <typename TAG>
	void Handle<TAG>::init(unsigned int index)
	{
		// Check if the handle is valid.
		assert(isNull());

		// Check that the index is within allocated range.
		assert(index <= HANDLE.MAX_INDEX);

		static unsigned int s_AutoMagic = 0;
		if (++s_AutoMagic > HANDLE.MAX_MAGIC)
		{
			// Value 0 is reserved for Null handle.
			s_AutoMagic = 1;
		}

		HANDLE.MAXBITS.m_Index = index;
		HANDLE.MAXBITS.m_Magic = s_AutoMagic;
	}

	// ----------------------------------------------------
	// Overwriting operators for simpler Handle comparisons.
	template <typename TAG>
	inline bool operator != (Handle <TAG> h1, Handle <TAG> h2)
	{
		return (h1.GetHandle() != h2.GetHandle());
	}

	template <typename TAG>
	inline bool operator == (Handle <TAG> h1, Handle <TAG> h2)
	{
		return (h1.GetHandle() == h2.GetHandle());
	}


}