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

		SceneManager* getSceneManager()
		{
			return sceneManager;
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
		sge::Renderer* renderer;

		bool running;
		float step;
		float accumulator;
	};
};