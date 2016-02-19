#include "Game\Scene.h"
#include <iostream>

namespace sge
{
	Scene::Scene()
	{
		std::cout << "Scene constructor says hello" << std::endl;
	}

	Scene::~Scene()
	{
		std::cout << "Scene destructor says hello" << std::endl;
	}

	void Scene::begin()
	{
		std::cout << "Scene begin says hello" << std::endl;
	}

	void Scene::end()
	{
		std::cout << "Scene end says hello" << std::endl;
	}
};