#include "Resources/Resource.h"


Resource::Resource(const std::string& resourcePath)
{
	this->resourcePath = resourcePath;
	references = 0;
}


Resource::~Resource()
{
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