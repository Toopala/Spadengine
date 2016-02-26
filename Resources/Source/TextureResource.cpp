#include "Resources/TextureResource.h"


namespace sge
{
	TextureResource::TextureResource(const std::string& resourcePath, Spade* engine) : sge::Resource(resourcePath), engine(engine)
	{
		path = resourcePath;
		unsigned char* data = stbi_load(resourcePath.c_str(), &width, &height, &comp, STBI_rgb_alpha);
		this->texture = engine->getDevice().createTexture(width, height, data);
		stbi_image_free(data);

		std::cout << "Texture loaded succesfully!" << std::endl;
	}


	TextureResource::~TextureResource()
	{
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

	std::string TextureResource::getPath()
	{
		return path;
	}

	std::string TextureResource::getTypeName()
	{
		return typeName;
	}

	void TextureResource::setTypename(const std::string& typeName)
	{
		this-> typeName = typeName;
	}
	
	unsigned int TextureResource::getId()
	{
		return id;
	}

	Texture* TextureResource::getTexture()
	{
		return texture;
	}
}
