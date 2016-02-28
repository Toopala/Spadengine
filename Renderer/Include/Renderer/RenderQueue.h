#pragma once

#include <vector>

#include "Core/Assert.h"
#include "Renderer/RenderCommand.h"

namespace sge
{
	struct RenderData;

	// TODO std::pair works but is it the fastest method? 
	using Queue = std::vector<std::pair<RenderCommand, const RenderData*>>;

	class RenderQueue
	{
	public:
		RenderQueue(size_t size);

		void begin();
		void end();
		void sort();

		inline const Queue& getQueue() const
		{ 
			return queue; 
		}
		
		inline void push(const RenderCommand command, const RenderData* data)
		{
			if (!acceptingCommands)
			{
				return;
			}

			queue.emplace_back(std::make_pair(command, data));
		}
	private:
		Queue queue;
		bool acceptingCommands;
	};
}