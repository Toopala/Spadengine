#pragma once

#include "stb_image.h"
#include "Renderer/Texture.h"
#include "Renderer/GraphicsDevice.h"
#include "Resources/Resource.h"
#include "Core/Math.h"


namespace sge
{
	/** \brief The class that handles textures.*/
	class TextureResource : public sge::Resource
	{
	public:
		/** \brief The constructor.
		*
		*	\param const std::string& resourcePath : The path to the texture that is going to be loaded.
		*/
		TextureResource(const std::string& resourcePath, GraphicsDevice* device);

		/** \brief The destructor.*/
		~TextureResource();

		/** \brief Gets the texture data.
		*
		*	\return Returns the pointer to the texture's data.
		*/
		unsigned char* getData();

		/** \brief Gets the size of the texture.
		*
		*	\return Returns the size of the texture in pixels.
		*/
		sge::math::ivec2 getSize();

		/** \brief Gets the typename.
		*
		*	\return Returns the type of the texture.
		*/
		std::string getTypeName();

		/** \brief Gets the texture.
		*
		*	\return Returns the texture.
		*/
		Texture getTexture();

		/** \brief Sets the typename.
		*
		*	\param const std::string& typeName : The name of the desired type.
		*/
		void setTypename(const std::string& typeName);

	private:
		int width;				/**<  Width of the texture. */
		int height;				/**<  Height of the texture. */
		int comp;				/**<  Number of components in texture. */
		std::string typeName;	/**<  Type of the texture. */
		unsigned char* data;	/**<  Texture data pointer. */
		Texture texture;		/**<  Texture class. */
	};
}