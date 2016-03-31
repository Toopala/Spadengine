#include "Resources/Resource.h"

namespace sge
{
	sge::Resource::Resource(const std::string& resourcePath)
	{
		this->resourcePath = resourcePath;
		references = 0;
	}


	sge::Resource::~Resource()
	{	
		std::cout << "Resource was deleted." << std::endl;
	}

	std::string& Resource::getResourcePath()
	{
		return resourcePath;
	}

	int Resource::getReferenceCount()
	{
		return references;
	}

	void Resource::increaseRef()
	{
		references++;
	}

	void Resource::decreaseRef()
	{
		references--;
	}
}