#include "Resources/TextureResource.h"


namespace sge
{
	TextureResource::TextureResource(const std::string& resourcePath) : sge::Resource(resourcePath)
	{
		data = stbi_load(resourcePath.c_str(), &width, &height, &comp, STBI_rgb_alpha);
	
		std::cout << "Texture loaded succesfully!" << std::endl;
	}


	TextureResource::~TextureResource()
	{
		stbi_image_free(data);
	}

	void TextureResource::bind()
	{
		//device.bindTexture(texture, 0);

		std::cout << "Texture bound succesfully!" << std::endl;
	}

	sge::math::ivec2 TextureResource::getSize()
	{
		return sge::math::ivec2(width, height);
	}

	std::string TextureResource::getTypeName()
	{
		return typeName;
	}

	void TextureResource::setTypename(const std::string& typeName)
	{
		this-> typeName = typeName;
	}
}
