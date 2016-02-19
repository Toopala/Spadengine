#pragma once
#include "Spade\Spade.h"
#include "Game\Scene.h"

namespace sge
{
	Spade::Spade() : running(true), accumulator(0.0f), step(0.0f)
	{
		

		window = new Window("Spade Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720);
		device = new GraphicsDevice(*window);
	}

	Spade::~Spade()
	{

	}

	void Spade::init()
	{
		std::cout << "Spade init says hello" << std::endl;
		device->init();
		step = 1.0f / 60.0f;
	}

	void Spade::quit()
	{
		std::cout << "Spade quit says hello" << std::endl;
		device->deinit();
	}	
	
	void Spade::run(Scene* scene)
	{
		float deltaTime = 0.0f;
		float newTime = 0.0f;
		float currentTime = SDL_GetTicks() / 1000.0f;

		sceneManager.change(scene);
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
		// Event manager stuff here
	}

	void Spade::update(float deltaTime)
	{
		accumulator += deltaTime;

		while(accumulator >= step)
		{
			sceneManager.update(step);
			accumulator -= step;

			// inputs here
		}

		sceneManager.interpolate(accumulator / step);
	}

	void Spade::draw()
	{
		sceneManager.draw();
	}
};
