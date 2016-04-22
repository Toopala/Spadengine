#pragma once
#include <vector>
#include <unordered_map>
#include "Core/Assert.h"
#include "Resources/Resource.h"

// RESOURCE MANAGER
//
// The resource manager for Spadengine is based on handles.
// To use it, you need to have the manager and the resource headers included.
// You need to create an instance of the manager (preferably just one), and
// to be able to access it from where its required.
//
// Below is an example on how to load a resource into a handle.
// sge::ResourceManager resMgr;
// sge::Handle<sge::TextureResource> texHandle;
// texHandle = resMgr.load<sge::TextureResource>("../Assets/spade.png");
//
// After this the resource can be accessed by using
// texHandle.getResource<sge::TextureResource>();
//
// The handles and the manager keep track of the resource instances
// and make sure they are not unloaded before all the references are cleared.
//
// When handle is no longer needed, release it to keep track of references.

namespace sge
{
	template<typename T>
	class Handle;

	class ResourceManager
	{
	public:

		ResourceManager();
		~ResourceManager();

		// This function generates a new resource into our Handle.
		template <typename T>
		Handle<T> load(const std::string &filename)
		{
			if (filename.empty())
			{
				std::cout << "Filename cannot be empty! Error loading resource." << std::endl;
				return Handle<T>();		// Returns a null handle which can be used for error checking.
			}

			Handle<T> handle(this);
			unsigned int index;

			// Create a new resource pointer of our template type.
			T* resource = new T(filename);

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

			// Assertion to make sure our resource "chain" doesn't break
			pathVec.push_back(filename);
			SGE_ASSERT(pathVec.at(index) == filename);

			// Finally we add +1 to our resource references.
			std::unordered_map<std::string, sge::Resource*>::iterator it;
			it = userData.find(filename);

			if (it != userData.end())
			{
				(*it).second->increaseRef();
			}

			return handle;
		};

		// Function for releasing the handle after it's become unnecessary.
		template <class T>
		void release(Handle<T> handle)
		{
			unsigned int index = handle.getIndex();

			SGE_ASSERT(index < pathVec.size());
			SGE_ASSERT(magicNumbers[index] == handle.getMagic()); // We make sure we are releasing the correct handle by comparing the magic numbers.

			magicNumbers[index] = 0;
			freeSlots.push_back(index); // A new free slot is added to our list.

			std::unordered_map<std::string, sge::Resource*>::iterator it;
			std::string filename = pathVec.at(index);
			it = userData.find(filename);

			if (it != userData.end()) // References are decreased after release.
			{
				(*it).second->decreaseRef();
			}

			if ((*it).second->getReferenceCount() == 0) // If references have reached 0, we'll remove resource from memory.
			{
				(*it).second->~Resource();
			}

			std::cout << filename << " | Handle released. References: " << (*it).second->getReferenceCount() << std::endl;
		};

		// Function to retrieve a resource pointer from our handle.
		template <typename T>
		T* getResource(sge::Handle<T>& handle)
		{
			std::string path = pathVec.at(handle.getIndex());

			std::unordered_map<std::string, sge::Resource*>::iterator it;
			it = userData.find(path);
			return static_cast<T*>((*it).second);
		}

		static ResourceManager& getMgr()
		{
			static ResourceManager resMgr;
			return resMgr;
		}

		ResourceManager(const ResourceManager&) = delete;
		void operator=(const ResourceManager&) = delete;

		void printResources();

	private:

		// Keeps track of the resource paths and pointers for comparison.
		std::unordered_map<std::string, sge::Resource*> userData;

		// Magic numbers are used as an ID to make sure we have the correct resource.
		std::vector<unsigned int> magicNumbers;

		// Free slots are used to optimise resource storing.
		std::vector<unsigned int> freeSlots;

		// Resource paths.
		std::vector<std::string> pathVec;

		// Deletes all loaded resources.
		void releaseAll();

	};

	// -------------------------------------
	// HANDLE

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

				// Size to compare against for SGE_ASSERTing deferences
				MAX_INDEX = (1 << MAX_BITS_INDEX) - 1,
				MAX_MAGIC = (1 << MAX_BITS_MAGIC) - 1,
			};

			struct
			{
				unsigned m_Index : MAX_BITS_INDEX;	// Index into resource array
				unsigned m_Magic : MAX_BITS_MAGIC;	// Magic number to check

			} MAXBITS;

			unsigned int m_Handle;
		} HANDLE;

		ResourceManager* refManager;

	public:

		template <typename T>
		T* getResource()
		{
			return static_cast<T*>(refManager->getResource<T>(*this));
		}

		// Creation sets the handle to null for error check purposes.
		// Handle needs to be initialized before it can be properly used.

		Handle() : refManager(nullptr) { HANDLE.m_Handle = 0; }

		Handle(ResourceManager* refManager) : refManager(refManager) { HANDLE.m_Handle = 0; }

		void init(unsigned int index);

		// Methods for managing our Handle

		unsigned int getIndex()		const
		{
			return (HANDLE.MAXBITS.m_Index);
		}

		unsigned int getMagic()		const
		{
			return (HANDLE.MAXBITS.m_Magic);
		}

		bool isNull()				const
		{
			return (!HANDLE.m_Handle);
		}

	};

	// Initialization gives our handle its index and magic numbers.
	template <typename TAG>
	void Handle<TAG>::init(unsigned int index)
	{
		// Check if the handle is valid and within allocated range.
		SGE_ASSERT(isNull());
		SGE_ASSERT(index <= HANDLE.MAX_INDEX);

		static unsigned int s_AutoMagic = 0;
		if (++s_AutoMagic > HANDLE.MAX_MAGIC)
		{
			// Value 0 is reserved for Null handle.
			s_AutoMagic = 1;
		}

		HANDLE.MAXBITS.m_Index = index;
		HANDLE.MAXBITS.m_Magic = s_AutoMagic;
	}
}