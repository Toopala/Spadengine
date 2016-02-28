
#include "Renderer/GraphicsDevice.h"
#include "Renderer/Renderer.h"
#include "Renderer/Window.h"

namespace sge
{
	Renderer::Renderer(Window& window) :
		device(nullptr)
	{
		device = new GraphicsDevice(window);

		device->init();
	}

	Renderer::~Renderer()
	{
		device->deinit();
	}
}