#pragma once

#include "Core/Math.h"

#include "Renderer/RenderQueue.h"

namespace sge
{
	class GraphicsDevice;
	class Window;

	class Renderer
	{
	public:
		Renderer(Window& window);
		~Renderer();

		GraphicsDevice& getDevice() const { return *device; }

		inline void pushCommand(const RenderCommand command, const RenderData* data)
		{
			queue.push(command, data);
		}

		void begin();
		void end();

	private:
		GraphicsDevice* device;
		RenderQueue queue;

		const size_t queueSize = 1000;

		// TODO hax.
		math::mat4 PV;
	};
}