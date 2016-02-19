#pragma once
#include "Game\Scene.h"
#include <vector>

namespace sge
{
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		enum SceneAction
		{
			NONE,
			CHANGE,
			PUSH,
			POP
		};

		void update(float step);
		void interpolate(float alpha);
		void draw();
		void push(Scene* scene);
		void pop();
		void change(Scene* scene);
		void handleScenes();
	private:
		std::vector<Scene*> scenes;

		SceneAction sceneAction;
		Scene* newScene;
	};
};