#pragma once

#include <vector>
#include <functional>
#include "Core/Assert.h"
#include "Renderer/RenderCommand.h"

namespace sge
{
	class GraphicsDevice;

	class RenderQueue
	{
	public:
		using RenderFunction = std::function<void(GraphicsDevice*)>;
		using Queue = std::vector<std::pair<RenderCommand, RenderFunction>>;

		RenderQueue(size_t size);

		void begin();
		void end();
		void sort();
		void clear();

		inline const Queue& getQueue() const
		{ 
			return queue; 
		}
		
		inline void push(const RenderCommand command, RenderFunction renderFunction)
		{
			if (!acceptingCommands)
			{
				return;
			}

			queue.emplace_back(std::make_pair(command, renderFunction));
		}
	private:
		Queue queue;
		bool acceptingCommands;
	};
}