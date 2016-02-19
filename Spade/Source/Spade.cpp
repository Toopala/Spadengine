#pragma once
#include "Spade\Spade.h"
#include "Game\Scene.h"

namespace sge
{
	Spade::Spade() : running(true), accumulator(0.0f), step(0.0f)
	{
#ifdef OPENGL4

		// TODO load these from an external file?
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		SDL_GL_SetAttribute(SDL_GL_STEREO, 0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

#ifdef _DEBUG
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif
#endif

		window = new Window("Spade Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720);
		device = new GraphicsDevice(*window);
		mouseInput = new sge::MouseInput();
		eventManager = new EventManager(mouseInput);
	}

	Spade::~Spade()
	{
		delete device;
		delete window;
	}

	void Spade::init()
	{
		std::cout << "Spade init says hello" << std::endl;
		window = new Window("Spade Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720);
		device = new GraphicsDevice(*window);		
		device->init();
		step = 1.0f / 60.0f;
	}

	void Spade::quit()
	{
		std::cout << "Spade quit says hello" << std::endl;
		device->deinit();
		delete mouseInput;

		SDL_Quit();
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
		if (eventManager->userQuit())
		{
			running = false;
		}

		eventManager->update();
	}

	void Spade::update(float deltaTime)
	{
		accumulator += deltaTime;

		while(accumulator >= step)
		{
			sceneManager.update(step);
			accumulator -= step;

			mouseInput->update();
		}

		sceneManager.interpolate(accumulator / step);
	}

	void Spade::draw()
	{
		sceneManager.draw();
	}
};
