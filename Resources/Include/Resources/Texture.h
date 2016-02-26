#pragma once
#include "Resources/Resource.h"

namespace sge
{
	class TextureSource : public Resource
	{
	public:
		TextureSource(const std::string& resourcePath);

		~TextureSource();

		void bind();
	};
}
