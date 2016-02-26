#pragma once
#include "Resources/Resource.h"
#include "Core/Math.h"

#include "stb_image.h"

namespace sge
{
	class TextureResource : public sge::Resource
	{
	public:
		TextureResource(const std::string& resourcePath);
		~TextureResource();

		void bind();
		sge::math::ivec2 getSize();

	private:
		int width, height, comp;
	};
}