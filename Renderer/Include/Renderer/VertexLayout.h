#pragma once

#include "Renderer/Enumerations.h"

namespace sge
{
	static const unsigned int MAX_NUMBER_OF_ELEMENTS = 15;


	struct VertexElement
	{
		size_t offset;
		size_t size;
		VertexSemantic semantic;
	};

	struct VertexLayoutDescription
	{
		size_t count;
		VertexElement elements[MAX_NUMBER_OF_ELEMENTS];

		// TODO clean
	};

	struct VertexLayout
	{
		size_t count;
		size_t stride;
		VertexElement elements[MAX_NUMBER_OF_ELEMENTS];

		// TODO clean
	};
}