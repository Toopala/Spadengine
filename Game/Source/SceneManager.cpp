#include "Game/SceneManager.h"

namespace sge
{
	SceneManager::SceneManager() : newScene(nullptr), sceneAction(NONE)
	{

	}

	SceneManager::~SceneManager()
	{
		for (auto scene : scenes)
		{
			delete scene;
		}

		scenes.clear();
	}

	void SceneManager::update(float step)
	{
		scenes.back()->update(step);
	}

	void SceneManager::interpolate(float alpha)
	{
		scenes.back()->interpolate(alpha);
	}

	void SceneManager::draw()
	{
		scenes.back()->begin();
		scenes.back()->draw();
		scenes.back()->end();
	}

	void SceneManager::push(Scene* scene)
	{
		newScene = scene;
		sceneAction = PUSH;
	}

	void SceneManager::pop()
	{
		sceneAction = POP;
	}

	void SceneManager::change(Scene* scene)
	{
		newScene = scene;
		sceneAction = CHANGE;
	}

	void SceneManager::handleScenes()
	{
		switch (sceneAction)
		{
		case PUSH:
		{
			if (newScene)
			{
				scenes.push_back(newScene);
			}
			break;
		}
		case POP:
		{
			if (!scenes.empty())
			{
				delete scenes.back();
				scenes.pop_back();
			}
			break;
		}
		case CHANGE:
		{
			if (newScene)
			{
				if (!scenes.empty())
				{
					delete scenes.back();
					scenes.pop_back();
				}

				scenes.push_back(newScene);
			}
			break;
		}
		case NONE:
		default:
		{
			break;
		}
		}

		newScene = nullptr;
		sceneAction = NONE;
	}
};
