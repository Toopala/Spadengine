#include "Game/SystemManager.h"
#include <typeinfo>

namespace sge
{

	SystemManager::SystemManager()
	{
		testSys = new TestSystem();
		physSys = new PhysicsSystem();
		tranSys = new TransformSystem();
	}


	SystemManager::~SystemManager()
	{
		delete testSys;
		delete physSys;
		delete tranSys;
	}

	void SystemManager::init()
	{
		systems.emplace(typeid(TestComponent).hash_code(), testSys);
		systems.emplace(typeid(InputComponent).hash_code(), testSys);
		systems.emplace(typeid(TransformComponent).hash_code(), tranSys);
		systems.emplace(typeid(PhysicsComponent).hash_code(), physSys);
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
		physSys->update();
		tranSys->update();
	}

}