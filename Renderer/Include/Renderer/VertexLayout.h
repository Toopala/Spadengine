#pragma once

namespace sge
{
	struct VertexElement
	{
		size_t offset;
		size_t size;
	};

	struct VertexLayoutDescription
	{
		size_t count;
		size_t elements[];

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