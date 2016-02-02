#ifdef OPENGL4

#include "glad/glad.h"

#include "Renderer/Buffer.h"

namespace sge
{
	struct GL4Buffer 
	{
		Buffer header;

		GLuint id;
		GLenum target;
		GLenum usage;
		GLuint size;
	};
}

#endif