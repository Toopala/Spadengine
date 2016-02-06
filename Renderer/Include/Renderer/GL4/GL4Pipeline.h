#ifdef OPENGL4

#pragma once

#include "glad/glad.h"

#include "Renderer/Pipeline.h"
#include "Renderer/VertexLayout.h"

namespace sge
{
	struct GL4Pipeline
	{
		Pipeline header;

		VertexLayout vertexLayout;

		GLuint program;
		GLuint vao;
		
		// Test. These are indexes for uniform block locations.
		GLuint* uniformBlocks;
	};
}

#endif