#pragma once
#include <iostream>
#include <string>

class Resource
{
	friend class ResourceManager;
public:
	Resource(const std::string& resourcePath);
	~Resource();

	std::string& getResourcePath();
	int getReferenceCount();

protected:

	std::string resourcePath;
	int references;
	void increaseRef();
	void decreaseRef();
};