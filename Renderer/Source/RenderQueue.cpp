#include <algorithm>

#include "Renderer/RenderQueue.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/GraphicsDevice.h"

namespace sge
{
	RenderQueue::RenderQueue(size_t size) :
		acceptingCommands(false)
	{
		// TODO use page pool allocator.
		queue.reserve(size);
	}

	void RenderQueue::begin()
	{
		acceptingCommands = true;
	}

	void RenderQueue::end()
	{
		acceptingCommands = false;
		sort();
	}

	void RenderQueue::sort()
	{
		std::sort(std::begin(queue), std::end(queue), 
			[](const std::pair<RenderCommand, RenderFunction>& lhs, const std::pair<RenderCommand, RenderFunction>& rhs)
		{
			return lhs.first.bits < rhs.first.bits;
		});
	}

	void RenderQueue::clear()
	{
		queue.clear();
	}
}