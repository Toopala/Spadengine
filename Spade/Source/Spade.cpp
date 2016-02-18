#pragma once
#include "Spade.h"

namespace sge
{
	Spade::Spade() : running(true), accumulator(0.0f), step(0.0f)
	{
		window = new Window("Spade Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720);
		device = new GraphicsDevice(*window);

		device->init();
	}

	Spade::~Spade()
	{

	}

	void Spade::quit()
	{
		
	}
	
	void Spade::run()
	{
		float deltaTime = 0.0f;
		float newTime = 0.0f;
		float currentTime = SDL_GetTicks() / 1000.0f;

		sceneManager.change();
		sceneManager.handleScenes();

		while (running)
		{
			newTime = SDL_GetTicks() / 1000.0f;
			deltaTime = std::min(newTime - currentTime, 0.25f);
			currentTime = newTime;

			handleEvents();
			update(deltaTime);
			draw();

			sceneManager.handleScenes();
		}
	}

	void Spade::handleEvents()
	{
		
	}

	void Spade::update(float deltaTime)
	{

	}
};
