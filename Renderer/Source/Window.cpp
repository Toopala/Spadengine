#include "Renderer/Window.h"
#include "SDL/SDL.h"

namespace sge
{
	Window::Window(const char* title, int x, int y, int width, int height) :
		window(SDL_CreateWindow(title, x, y, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL))
	{
	}

	Window::~Window()
	{
		SDL_DestroyWindow(window);
	}

	void Window::swap()
	{
		SDL_GL_SwapWindow(window);
	}

	SDL_Window* Window::getSDLWindow()
	{
		return window;
	}
}