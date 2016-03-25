#pragma once

#include <cstddef>

#include "Core/Math.h"
#include "Renderer/RenderQueue.h"
#include "Renderer/GraphicsDevice.h"

namespace sge
{
	class GraphicsDevice;
	class Window;
	struct Buffer;

	class Renderer
	{
	public:
		Renderer(Window& window);
		~Renderer();

		void init();
		void deinit();

		GraphicsDevice& getDevice() const { return *device; }

		inline void pushCommand(const RenderCommand command, const RenderQueue::RenderFunction renderFunction)
		{
			queue.push(command, renderFunction);
		}

		void begin();
		void end();

	private:
		GraphicsDevice* device;
		RenderQueue queue;

		const size_t queueSize = 1000;
	};
}
