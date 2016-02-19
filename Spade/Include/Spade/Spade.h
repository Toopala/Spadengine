#pragma once

#include <iostream>
#include <algorithm>
#include "Renderer/Window.h"
#include "Game/SceneManager.h"
#include "Renderer/GraphicsDevice.h"
#include "SDL/SDL.h"

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

		const GraphicsDevice& getDevice(){ return *device; }

		SceneManager& getSceneManager()
		{
			return sceneManager;
		}

		const float getStep() const
		{
			return step;
		}

	private:
		void handleEvents();
		void update(float deltaTime);
		void draw();


		sge::GraphicsDevice* device;
		sge::Window* window;
		sge::SceneManager sceneManager;

		bool running;
		float step;
		float accumulator;
	};
};