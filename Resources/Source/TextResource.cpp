#include "Resources/TextResource.h"

namespace sge
{
	TextResource::TextResource(const std::string& resourcePath) : sge::Resource(resourcePath)
	{
		loadFont(resourcePath);
	}

	TextResource::~TextResource()
	{
	}

	void TextResource::loadFont(const std::string& resourcePath)
	{
		error = FT_Init_FreeType(&library);
		if (error)
		{
			std::cout << "An error occurred during library initialization." << std::endl;
		}

		error = FT_New_Face(library, resourcePath.c_str(), 0, &face);
		if (error == FT_Err_Unknown_File_Format)
		{
			std::cout << "The Text file is not supported." << std::endl;
		}
		else if (error)
		{
			std::cout << "The Text file could not be read. Please, check the filepath." << std::endl;
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

	std::string TextResource::getText()
	{
		return text;
	}

	float TextResource::getCharacterSize()
	{
		return characterSize;
	}
}