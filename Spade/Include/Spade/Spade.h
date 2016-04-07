#pragma once

#include <iostream>
#include <algorithm>
#include "Renderer/Window.h"
#include "Game/SceneManager.h"

#include "SDL2/SDL.h"
#include "Game/EventManager.h"
#include "Renderer/Renderer.h"
#include "Resources/ResourceManager.h"

namespace sge
{
	class Spade
	{
	public:
		Spade();
		~Spade();

		void init();
		void run(Scene* scene);
		void stop(){ running = false; }

		void quit();

        static Spade& getInstance()
        {
            static Spade spade;
            return spade;
        }

        Spade(const Spade&) = delete;
        void operator=(const Spade&) = delete;

		SceneManager* getSceneManager()
		{
			return sceneManager;
		}

		ResourceManager* getResourceManager()
		{
			return resourceManager;
		}

		Renderer* getRenderer()
		{
			return renderer;
		}

		const float getStep() const
		{
			return step;
		}

		sge::MouseInput* mouseInput;
		sge::KeyboardInput* keyboardInput;
		sge::GamepadInput* gamepadInput;

	private:
		void handleEvents();
		void update(float deltaTime);
		void draw();

		sge::Window* window;
		sge::SceneManager* sceneManager;
		sge::EventManager* eventManager;
		sge::ResourceManager* resourceManager;
		sge::Renderer* renderer;

		bool running;
		float step;
		float accumulator;
	};
};