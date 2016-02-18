#include "Renderer/Window.h"
#ifdef _WIN32
# include "SDL/SDL.h"
#else
# include "SDL2/SDL.h"
#endif

namespace sge
{
	Window::Window(const char* title, int x, int y, int width, int height) :
		width(width),
		height(height)
	{
		Uint32 flags = SDL_WINDOW_SHOWN;

#if defined(OPENGL4)
		flags |= SDL_WINDOW_OPENGL;
#endif
		window = SDL_CreateWindow(title, x, y, width, height, flags);
	}

	Window::~Window()
	{
		SDL_DestroyWindow(window);
	}

	SDL_Window* Window::getSDLWindow()
	{
		return window;
	}
}
