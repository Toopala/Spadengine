#pragma once

namespace sge
{
	struct Buffer;

	struct RenderData
	{
		// TODO Render data should not own these buffers.
		// This is just a temporary structure needed for runtime rendering.

		std::vector<Buffer*> buffers;
		size_t count;
	};
}