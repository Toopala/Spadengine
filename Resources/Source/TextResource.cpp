#include "Resources/TextResource.h"

namespace sge
{
	TextResource::TextResource(const std::string& resourcePath) : sge::Resource(resourcePath)
	{
		error = FT_Init_FreeType(&library); // Initializes FreeType library
		if (error)
		{
			std::cout << "An error occurred during library initialization." << std::endl;
		}

		setFont(resourcePath);
	}

	TextResource::~TextResource()
	{
	
	}

	// Setters:

	void TextResource::setFont(const std::string& resourcePath)
	{
		error = FT_New_Face(library, resourcePath.c_str(), 0, &face); // Loads font file
		if (error == FT_Err_Unknown_File_Format)
		{
			std::cout << "The font file is not supported." << std::endl;
		}
		else if (error)
		{
			std::cout << "The font file could not be read. Please, check the filepath." << std::endl;
		}

		setCharacterSize(characterSize);
	}

	void TextResource::setText(std::string message)
	{
		text = message;
	}

	void TextResource::setCharacterSize(float size)
	{
		error = FT_Set_Char_Size(face, 64 * characterSize, 64 * characterSize, 300, 300);
		if (error)
		{
			std::cout << "An error occurred while trying to set character size." << std::endl;
		}
	}

	void TextResource::setColor(sge::math::vec4 c)
	{
		textColor = c;
	}

	void TextResource::setPosition(sge::math::vec3 p)
	{
		textPosition = p;
	}

	void TextResource::setScale(sge::math::vec2 s)
	{
		textScale = s;
	}

	void TextResource::setRotation(float a)
	{
		textAngle = a;
	}

	void TextResource::move(sge::math::vec3 p)
	{
		textPosition += p;
	}

	void TextResource::scale(sge::math::vec2 s)
	{
		textScale += s;
	}

	void TextResource::rotate(float a)
	{
		textAngle += a;
	}

	// Getters:

	std::string TextResource::getText()
	{
		return text;
	}

	float TextResource::getCharacterSize()
	{
		return characterSize;
	}

	sge::math::vec4 TextResource::getColor()
	{
		return textColor;
	}

	sge::math::vec3 TextResource::getPosition()
	{
		return textPosition;
	}

	sge::math::vec2 TextResource::getScale()
	{
		return textScale;
	}

	float TextResource::getRotation()
	{
		return textAngle;
	}

	sge::math::vec2 TextResource::getBounds() // Returns the width and height of the text that is currently set
	{
		sge::math::vec2 pen = { 0, 0 };
		if (!error)
		{
			for (int i = 0; i < text.size(); i++)
			{
				FT_Load_Char(face, text[i], FT_LOAD_RENDER);
				pen.x += face->glyph->advance.x >> 6; // Adds the width of current character to the total width

				if (face->glyph->bitmap.rows > pen.y) // Checks if the height of the current character is bigger than the one that is previously stored, if true -> replace the previous value
				{
					pen.y = face->glyph->bitmap.rows;
				}
			}
		}
		return pen;
	}

	sge::math::vec2 TextResource::getGlobalBounds()
	{
		return getBounds() * textScale;
	}
}