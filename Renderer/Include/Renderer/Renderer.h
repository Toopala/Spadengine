#pragma once

#include <cstddef>

#include "Core/Math.h"
#include "Renderer/RenderQueue.h"

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
		Buffer* uniformBuffer;
		math::mat4 PV;
		struct UniformData
		{
			math::mat4 MVP;
			math::vec4 color;
		} uniformData;
	};
}