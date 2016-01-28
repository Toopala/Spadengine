#pragma once

#include "Renderer/Enumerations.h"

namespace sge
{
	struct VertexElement
	{
		VertexFormat vertexFormat;
	};

	struct VertexLayout
	{
		size_t stride;
		size_t vertexElementCount;
		VertexElement *vertexElements;
	};

	struct VertexLayoutDescription
	{
		size_t count;
		VertexElement vertexElements[MAX_VERTEX_INPUTS];
	};
}