#include "Resources/FontResource.h"

namespace sge
{
	FontResource::FontResource(const std::string& resourcePath) : sge::Resource(resourcePath)
	{
		error = FT_Init_FreeType(&library); // Initializes FreeType library
		if (error)
		{
			std::cout << "An error occurred during library initialization." << std::endl;
		}

		setFont(resourcePath);
	}

	FontResource::~FontResource()
	{
		FT_Done_FreeType(library);
		FT_Done_Face(font.face);
	}

	void FontResource::setFont(const std::string& resourcePath)
	{
		error = FT_New_Face(library, resourcePath.c_str(), 0, &font.face); // Loads FontResource file
		if (error == FT_Err_Unknown_File_Format)
		{
			std::cout << "The FontResource file is not supported." << std::endl;
		}
		else if (error)
		{
			std::cout << "The FontResource file could not be read. Please, check the filepath." << std::endl;
		}

		setCharacterSize(font.characterSize);
	}

	void FontResource::setCharacterSize(float size)
	{
		error = FT_Set_Char_Size(font.face, 64 * size, 64 * size, 300, 300);
		if (error)
		{
			std::cout << "An error occurred while trying to set character size." << std::endl;
		}
	}

	Font* FontResource::getFont()
	{
		return &font;
	}

	float FontResource::getCharacterSize()
	{
		return font.characterSize;
	}
}