
#include "Renderer/GraphicsDevice.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderData.h"
#include "Renderer/Window.h"

namespace sge
{
	Renderer::Renderer(Window& window) :
		device(nullptr),
		queue(queueSize)
	{
		device = new GraphicsDevice(window);

		device->init();
	}

	Renderer::~Renderer()
	{
		device->deinit();

		delete device;
	}

	void Renderer::begin()
	{
		queue.begin();
	}

	void Renderer::end()
	{
		queue.end();

		device->clear(1.0f, 0.0f, 0.2f, 1.0f);

		for (auto command : queue.getQueue())
		{
			// TODO hax bind vertex buffer.
			device->bindVertexBuffer(command.second->buffers[0]);

			// TODO hax draw to screen.
			device->draw(command.second->count);
		}

		queue.clear();

		device->swap();
	}
}