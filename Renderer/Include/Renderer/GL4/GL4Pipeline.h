#ifdef OPENGL4

#include "glad/glad.h"

#include "Renderer/Pipeline.h"

namespace sge
{
	struct GL4Pipeline
	{
		Pipeline header;
		GLuint program;
		GLuint vao;
	};
}

#endif