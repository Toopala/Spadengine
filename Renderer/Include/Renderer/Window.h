#pragma once

struct SDL_Window;

namespace sge
{
	class Window
	{
	public:
		Window(const char* title, int x, int y, int width, int height);
		~Window();

		void swap();
		SDL_Window* getSDLWindow();
	private:
		SDL_Window* window;
	};
}