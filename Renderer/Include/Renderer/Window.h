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

		int getWidth() 
		{
			return width;
		}
		int getHeight()
		{
			return height;
		}

	private:
		SDL_Window* window;
		int width;
		int height;
	};
}