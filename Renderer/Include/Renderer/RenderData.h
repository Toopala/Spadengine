#pragma once

#include "Core/Math.h"

namespace sge
{
	struct Buffer;

	struct RenderData
	{
		// TODO Render data should not own these buffers.
		// This is just a temporary structure needed for runtime rendering.

		std::vector<Buffer*> buffers;
		size_t count;

		// TODO need a smarter and more generic way to pass this data.
		math::vec3 pos;
	};
}