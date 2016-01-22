#include "Core/Math.h"
#include "Renderer/Window.h"
#include "Renderer/GraphicsDevice.h"
#include "SDL/SDL.h"

#include <iostream>

int main(int argc, char** argv)
{

	SDL_Init(SDL_INIT_VIDEO);

	sge::math::vec2 vec;
	std::cout << vec.x << ", " << vec.y << ", " << sge::math::haeSata() << std::endl;

	sge::Window window("Spade Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720);
	sge::GraphicsDevice device(window);
	device.setClearColor(0.5f, 0.0f, 0.5f, 1.0f);

	SDL_Event event;

	bool running = true;

	while (running)
	{
		device.clear();

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			}
		}

		window.swap();
	}

	SDL_Quit();

	return 0;
}