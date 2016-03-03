#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

#include "Resources/Resource.h"

namespace sge
{
	class TextResource : public sge::Resource
	{
	public:
		TextResource(const std::string& resourcePath);
		~TextResource();

		void loadFont(const std::string& resourcePath);
		void setText(std::string message);
		void setCharacterSize(float size);

		std::string getText();
		float getCharacterSize();

	private:
		FT_Library library;
		FT_Face face;
		FT_Error error;

		float characterSize = 12;
		std::string text = "";
	};
}