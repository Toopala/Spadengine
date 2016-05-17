#ifdef OPENGL4

#pragma once

#include "glad/glad.h"

#include "Renderer/CubeMap.h"

namespace sge
{
	struct GL4CubeMap
	{
		CubeMap header;

		GLuint id;
	};
}

#endif