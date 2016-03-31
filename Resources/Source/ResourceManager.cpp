#include "Resources/ResourceManager.h"

namespace sge
{
	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager()
	{
	}

	void ResourceManager::printResources()
	{
		for (auto resource : userData)
		{
			std::cout << resource.first << ": "
				<< resource.second->getReferenceCount() << " references" << std::endl;
		}
	}

	void ResourceManager::releaseAll()
	{
		for (auto resource : userData)
		{
			resource.second->~Resource();
			std::cout << resource.first << "released." << std::endl;
		}
	}
}