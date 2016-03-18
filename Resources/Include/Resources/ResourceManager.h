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
		sge::Handle<T> load(const std::string &filename)
		{
			if (filename.empty())
			{
				std::cout << "Filename cannot be empty! Error loading resource." << std::endl;
				return sge::Handle<T>();
			}

			sge::Handle<T> handle(this);
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
}
