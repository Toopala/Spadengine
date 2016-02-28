#include <algorithm>

#include "Renderer/RenderQueue.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/RenderData.h"

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
		std::sort(queue.begin(), queue.end(), 
			[](const std::pair<RenderCommand, const RenderData*>& lhs, const std::pair<RenderCommand, const RenderData*>& rhs)
		{
			return lhs.first.bits < rhs.first.bits;
		});
	}
}