#pragma once
#include <vector>
#include <cassert>
#include <unordered_map>
#include "Resources/Resource.h"
#include "Resources/TextureResource.h"
#include "Resources/Handle.h"

namespace sge
{
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

			Handle<T> handle;
			unsigned int index;

			// TODO STRING HASHING
			std::unordered_map<std::string, Resource*>::iterator it;
			it = userData.find(filename);

			if (it != userData.end())
			{
				(*it).second->increaseRef();

			}

			T* resource = new T(filename);

			// TODO REFERENCE COUNTING WITH MAP

			if (freeSlots.empty())
			{
				index = magicNumbers.size();
				handle.init(index);
				//	userData.push_back(resource);
				magicNumbers.push_back(handle.getMagic());
			}
			else
			{
				index = freeSlots.back();
				handle.init(index);
				freeSlots.pop_back();
				magicNumbers[index] = handle.getMagic();
			}

			return handle;
		};


		template <class T>
		void release(Handle<T> handle)
		{
			unsigned int index = handle.getIndex();

			// TODO REFERENCE COUNTING WITH MAP

			//assert(index < userData.size());
			assert(magicNumbers[index] == handle.getMagic());

			magicNumbers[index] = 0;
			freeSlots.push_back(index);

			std::cout << "Handle was released." << std::endl;
		};


		bool unload(const std::string &filename);
		void printResources();

	private:

		std::unordered_map<std::string, Resource*> userData;
		std::vector<unsigned int> magicNumbers;
		std::vector<unsigned int> freeSlots;

		void releaseAll();

	};
}
