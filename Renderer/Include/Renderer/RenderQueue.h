#pragma once

#include <unordered_map>

#include "Renderer/RenderCommand.h"

namespace sge
{
	struct RenderData;

	using Queue = std::unordered_map<uint64, const RenderData*>;

	class RenderQueue
	{
	public:
		void begin();
		void end();
		const Queue& getQueue() { return queue; }
		void sort();
		void push(const RenderCommand command, const RenderData* data)
		{
			queue[command.bits] = data;
		}
	private:
		Queue queue;
	};
}