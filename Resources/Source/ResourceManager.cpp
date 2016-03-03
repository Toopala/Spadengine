#include "Resources/ResourceManager.h"

namespace sge
{
	ResourceManager::ResourceManager()
	{
	}


	ResourceManager::~ResourceManager()
	{
	}

	bool ResourceManager::unload(const std::string &filename)
	{
		if (filename.empty())
		{
			std::cout << "Filename cannot be empty! Error loading resource." << std::endl;
			return false;
		}
		return true;
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

	}
}