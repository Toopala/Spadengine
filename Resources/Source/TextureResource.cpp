#include "Resources/TextureResource.h"

namespace sge
{
	TextureResource::TextureResource(const std::string& resourcePath) : sge::Resource(resourcePath)
	{
#ifdef OPENGL4
        // Flips texture rows for opengl.
        stbi_set_flip_vertically_on_load(true);
#endif
		data = stbi_load(resourcePath.c_str(), &width, &height, &format, STBI_rgb_alpha);

        if (!data)
        {
            std::cout << "Error loading texture " << resourcePath << " : " << stbi_failure_reason() << std::endl;
        }
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

	Texture TextureResource::getTexture()
	{
		return texture;
	}

	void TextureResource::setTypename(const std::string& type)
	{
		typeName = type;
	}

    int TextureResource::getFormat()
    {
        return format;
    }
}
