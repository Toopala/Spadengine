
#include "Renderer/Buffer.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderData.h"
#include "Renderer/Window.h"

namespace sge
{
	Renderer::Renderer(Window& window) :
		queue(queueSize)
	{
		device = new GraphicsDevice(window);
	}

	Renderer::~Renderer()
	{
		delete device;
	}

	void Renderer::init()
	{
		device->init();
	}

	void Renderer::deinit()
	{
		device->deinit();
	}

	void Renderer::begin()
	{
		queue.begin();
	}

	void Renderer::end()
	{
		queue.end();

		device->clear(0.5f, 0.6f, 0.2f, 1.0f);

		for (auto& command : queue.getQueue())
		{
			command.second(device);
		}

		queue.clear();

		device->swap();
	}
}