#include "Resources/Texture.h"

namespace sge
{
	TextureSource::TextureSource(const std::string& resourcePath) : Resource(resourcePath)
	{
		std::cout << "Texture loaded succesfully!" << std::endl;
	}


	TextureSource::~TextureSource()
	{
	}


	void TextureSource::bind()
	{
		std::cout << "Texture bound succesfully!" << std::endl;
	}
}