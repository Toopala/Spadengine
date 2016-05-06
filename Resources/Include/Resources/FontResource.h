#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

#include "Resources/Resource.h"
#include "Core/Math.h"

namespace sge
{
	struct Font
	{
		FT_Face face;
		float characterSize = 12;
	};

	class FontResource : public sge::Resource
	{
	public:
		FontResource(const std::string& resourcePath);
		~FontResource();

		void setFont(const std::string& resourcePath);
		void setCharacterSize(float size);

		Font* getFont();
		float getCharacterSize();

	private:
		Font font;
		FT_Library library;
		FT_Error error;
	};
}