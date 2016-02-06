#ifdef OPENGL4

#pragma once

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