
#include "Renderer/Buffer.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderData.h"
#include "Renderer/Window.h"

namespace sge
{
	Renderer::Renderer(Window& window) :
		queue(queueSize),
		uniformBuffer(nullptr),
		PV(1.0f)
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

		// TODO hax these matrices shouldn't be here.
		// Also fixed window size is no good.
		PV = math::ortho(0.0f, 1280.0f, 720.0f, 0.0f);

		// TODO hax size should be something nicer. Now this only supports one struct.
		uniformBuffer = device->createBuffer(BufferType::UNIFORM, BufferUsage::DYNAMIC, sizeof(uniformData));
	}

	void Renderer::deinit()
	{
		device->deleteBuffer(uniformBuffer);
		device->deinit();
	}

	void Renderer::begin()
	{
		queue.begin();
	}

	void Renderer::end()
	{
		queue.end();

		device->clear(1.0f, 0.0f, 0.2f, 1.0f);

		// TODO hax bind uniform buffer and copy data. Probably not the best method here.
		device->bindVertexUniformBuffer(uniformBuffer, 0);

		for (auto& command : queue.getQueue())
		{
			// TODO parse this command so renderer has some idea what to do with it.
			// For example if its a command command ":D" you execute it and if it's
			// something to render you render it.
			
			// TODO some optimization should take place here. Can we combine data 
			// used by same shaders? For example. RESEARCH NEEDED!

			// TODO hax bind vertex buffer. Now we just bind the first buffer in the list. 
			// Add more generic system.
			device->bindVertexBuffer(command.second->buffers[0]);

			uniformData.MVP = PV * math::translate(math::mat4(1.0f), command.second->pos);
			uniformData.color = command.second->color;

			device->copyData(uniformBuffer, sizeof(uniformData), &uniformData);

			// TODO hax draw to screen.
			device->draw(command.second->count);
		}

		queue.clear();

		device->swap();
	}
}