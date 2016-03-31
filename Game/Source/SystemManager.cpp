#include "Game/SystemManager.h"
#include <typeinfo>

namespace sge
{

	SystemManager::SystemManager()
	{
		
	}


	SystemManager::~SystemManager()
	{
		delete testSys;
	}

	void SystemManager::init()
	{
		testSys = new TestSystem();

		systems.emplace(typeid(TestComponent).hash_code(), testSys);
		systems.emplace(typeid(InputComponent).hash_code(), testSys);
	}

	void SystemManager::addToSystem(Component* comp)
	{
		for (auto& system : systems)
		{
			if (typeid(*comp).hash_code() == system.first)
			{
				system.second->addComponent(comp);
			}
		}
	}

	void SystemManager::updateSystems()
	{
		testSys->update();
	}

}