#include "Spade/Spade.h"
#include "Game/Scene.h"

namespace sge
{
	Spade::Spade() : 
        window("Spade Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720),
        renderer(window),
        running(true), 
        accumulator(0.0f), 
        step(0.0f)
	{
#ifdef OPENGL4

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
	}

	Spade::~Spade()
	{
	}

	void Spade::init()
	{
		renderer.init();
		step = 1.0f / 60.0f;

		mouseInput = new sge::MouseInput();
		keyboardInput = new sge::KeyboardInput();
		gamepadInput = new sge::GamepadInput();
		eventManager = new EventManager(mouseInput, keyboardInput, gamepadInput);
		sceneManager = new SceneManager();
	}

	void Spade::quit()
	{
		delete eventManager;
		delete sceneManager;
		delete mouseInput;
		delete keyboardInput;
        delete gamepadInput;

		SDL_Quit();
	}	
	
	void Spade::run(Scene* scene)
	{
		float deltaTime = 0.0f;
		float newTime = 0.0f;
		float currentTime = SDL_GetTicks() / 1000.0f;

		sceneManager->change(scene);
		sceneManager->handleScenes();

		while (running)
		{
			newTime = SDL_GetTicks() / 1000.0f;
			deltaTime = std::min(newTime - currentTime, 0.25f);
			currentTime = newTime;

			handleEvents();
			update(deltaTime);
			draw();

			sceneManager->handleScenes();
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
			sceneManager->update(step);
			accumulator -= step;

			mouseInput->update();
			keyboardInput->update();
			gamepadInput->update();
		}

		sceneManager->interpolate(accumulator / step);
	}

	void Spade::draw()
	{
		sceneManager->draw();
	}
};
