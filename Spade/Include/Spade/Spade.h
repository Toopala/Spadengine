#pragma once

#include <iostream>
#include <algorithm>
#include "Renderer/Window.h"
#include "Resources/ResourceManager.h"

#include "Game/EventManager.h"
#include "Game/RenderSystem.h"
#include "Game/SceneManager.h"

#include "SDL2/SDL.h"

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

		SceneManager* getSceneManager()
		{
			return sceneManager;
		}

        RenderSystem* getRenderer()
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
        sge::RenderSystem* renderer;

		bool running;
		float step;
		float accumulator;
	};
};