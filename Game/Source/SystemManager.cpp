#include "Game/SystemManager.h"
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

	void SystemManager::addComponent(Component* component)
	{
		for (auto& system : systems)
		{
			if (typeid(*component).hash_code() == system.first)
			{
				system.second->addComponent(component);
			}
		}
	}

	void SystemManager::addSystem(System* system, size_t type)
	{
		systems.emplace(type, system);
	}

	void SystemManager::updateSystems()
	{
		for (auto& system : systems)
		{
			system.second->update();
		}
	}

}