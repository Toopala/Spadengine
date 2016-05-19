#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

#include "Resources/Resource.h"
#include "Core/Math.h"

namespace sge
{
	/** \brief Struct where font information is stored. */
	struct Font
	{
		FT_Face face;				/**<  Contains characters in current font and their variables. */
		float characterSize = 12;	/**<  Size of the characters. Character size of 12 is the default and the size of actual characters in pixels varies depending on the font. */
	};

	/** \brief The class that is used to load fonts. */
	class FontResource : public sge::Resource
	{
	public:
		/** \brief The constructor.
		*
		*	\param const std::string& resourcePath : Font's filepath.
		*/
		FontResource(const std::string& resourcePath);

		/** \brief The destuctor. */
		~FontResource();

		/** \brief Can be used to switch to an another font.
		*
		*	\param const std::string& resourcePath : Font's filepath.
		*/
		void setFont(const std::string& resourcePath);

		/** \brief Sets the character size to desired value.
		*
		*	\param float size : The desired size for characters. The actual size of characters depends on font.
		*/
		void setCharacterSize(float size);

		/** \brief Getter function for font struct.
		*
		*	\return Returns a font struct that contains the face and character size.
		*/
		Font* getFont();

		/** \brief Getter function for current character size.
		*
		*	\return Returns current character size.
		*/
		float getCharacterSize();

	private:
		Font font;				/**<  Current font and its character size. */
		FT_Library library;		/**<  FreeType library. Has to be initialized before accessing fonts. */
		FT_Error error;			/**<  Error check. */
	};
}