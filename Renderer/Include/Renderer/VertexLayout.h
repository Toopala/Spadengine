#pragma once

namespace sge
{
	static const unsigned int MAX_NUMBER_OF_ELEMENTS = 5;

	struct VertexElement
	{
		size_t offset;
		size_t size;
	};

	struct VertexLayoutDescription
	{
		size_t count;
		size_t elements[MAX_NUMBER_OF_ELEMENTS];

		// TODO clean
	};

	struct VertexLayout
	{
		size_t count;
		size_t stride;
		VertexElement* elements;

		// TODO clean
	};
}