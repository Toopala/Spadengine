#ifdef OPENGL4

#include "glad/glad.h"

#include "Renderer/Shader.h"

namespace sge
{
	struct GL4Shader
	{
		Shader header;

		GLuint id;
		GLenum type;
	};
}

#endif