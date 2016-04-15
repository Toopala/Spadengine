#include "Resources/TextureResource.h"

namespace sge
{
	TextureResource::TextureResource(const std::string& resourcePath) : sge::Resource(resourcePath)
	{
		data = stbi_load(resourcePath.c_str(), &width, &height, &comp, STBI_rgb_alpha);
	}

	TextureResource::~TextureResource()
	{
		//stbi_image_free(data);
	}

	unsigned char* TextureResource::getData()
	{
		return data;
	}

	sge::math::ivec2 TextureResource::getSize()
	{
		return sge::math::ivec2(width, height);
	}

	std::string TextureResource::getTypeName()
	{
		return typeName;
	}

	void TextureResource::setTypename(const std::string& type)
	{
		typeName = type;
	}
}
