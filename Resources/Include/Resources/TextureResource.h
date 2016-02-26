#pragma once
#include "Resources/Resource.h"
#include "Core/Math.h"

#include "stb_image.h"
#include "Renderer/Texture.h"
#include "Spade/Spade.h"

namespace sge
{
	class TextureResource : public sge::Resource
	{
	public:
		TextureResource(const std::string& resourcePath, Spade* engine);
		~TextureResource();

		void bind();
		sge::math::ivec2 getSize();

		std::string getPath();
		std::string getTypeName();
		unsigned int getId();

		void setTypename(const std::string& typeName);
		void setId(unsigned int id);
		Texture* getTexture();
	private:
		int width, height, comp;
		std::string path;
		std::string typeName;
		unsigned int id;
		Texture* texture;
		Spade* engine;
	};
}