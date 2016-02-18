#ifdef OPENGL4

#pragma once

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
	};
}

#endif