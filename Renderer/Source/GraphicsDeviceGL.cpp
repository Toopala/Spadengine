#ifndef DIRECTX_MODE

#include <iostream>
#include "Renderer/GraphicsDevice.h"
#include "Renderer/Window.h"
#include "SDL/SDL.h"
#include "glad/glad.h"

namespace sge
{
	class GraphicsDevice::Impl
	{
	public:
		Impl(Window& window) :
			context(SDL_GL_CreateContext(window.getSDLWindow()))
		{
			if (!gladLoadGL())
			{
				std::cout << "EI VITTU TOIMI!" << std::endl;
			}
		}

		~Impl()
		{
			SDL_GL_DeleteContext(context);
		}

		void clear()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}

		void setClearColor(float r, float g, float b, float a)
		{
			glClearColor(r, g, b, a);
		}

	private:
		SDL_GLContext context;
	};

	GraphicsDevice::GraphicsDevice(Window& window) :
		impl(new Impl(window))
	{
	}

	GraphicsDevice::~GraphicsDevice()
	{
		delete impl;
	}

	void GraphicsDevice::clear()
	{
		impl->clear();
	}

	void GraphicsDevice::setClearColor(float r, float g, float b, float a)
	{
		impl->setClearColor(r, g, b, a);
	}
}

#endif