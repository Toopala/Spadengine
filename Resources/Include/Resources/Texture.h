#pragma once
#include "Resource.h"


class Texture : public Resource
{
public:
	Texture(const std::string& resourcePath);

	~Texture();

	void bind();
};

