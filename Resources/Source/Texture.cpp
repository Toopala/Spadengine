#include "Resources/Texture.h"


Texture::Texture(const std::string& resourcePath) : Resource(resourcePath)
{
	std::cout << "Texture loaded succesfully!" << std::endl;
}


Texture::~Texture()
{
}


void Texture::bind()
{
	std::cout << "Texture bound succesfully!" << std::endl;
}