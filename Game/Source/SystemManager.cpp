#include "Game/SystemManager.h"
#include "Spade/Spade.h"
#include "Renderer/Renderer.h"
#include "Game/TestSystem.h"
#include "Game/PhysicsSystem.h"
#include "Game/TransformSystem.h"
#include "Game/SpriteRenderingSystem.h"
#include "Game/TestComponent.h"
#include "Game/PhysicsComponent.h"
#include "Game/TransformComponent.h"
#include "Game/SpriteComponent.h"

#include <typeinfo>

namespace sge
{

	SystemManager::SystemManager() : testSys(nullptr), physSys(nullptr), tranSys(nullptr), spritSys(nullptr)
	{

	}

	SystemManager::~SystemManager()
	{
		delete testSys;
		delete physSys;
		delete tranSys;
		delete spritSys;
	}

	void SystemManager::init(Renderer* renderer)
	{
		testSys = new TestSystem();
		physSys = new PhysicsSystem();
		tranSys = new TransformSystem();
		spritSys = new SpriteRenderingSystem(renderer);
		systems.emplace(typeid(TestComponent).hash_code(), testSys);
		systems.emplace(typeid(InputComponent).hash_code(), testSys);
		systems.emplace(typeid(TransformComponent).hash_code(), tranSys);
		systems.emplace(typeid(PhysicsComponent).hash_code(), physSys);
		systems.emplace(typeid(SpriteComponent).hash_code(), spritSys);
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
		spritSys->update();
	}

}