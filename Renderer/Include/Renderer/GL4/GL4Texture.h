#ifdef OPENGL4

#include "glad/glad.h"

#include "Renderer/Texture.h"

namespace sge
{
	struct GL4Texture
	{
		Texture header;

		GLuint id;
	};
}

#endif