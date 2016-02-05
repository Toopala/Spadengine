#include "Game/EntityManager.h"
#include <iostream>
namespace sge
{
	EntityManager::EntityManager()
	{
	}


	EntityManager::~EntityManager()
	{
	}

	Entity* EntityManager::createEntity() // Creates an empty entity.
	{
		return new Entity();
	}

	Entity* EntityManager::createActor() // Creates an entity with predetermined components.
	{
		Entity* actor = new Entity();
		actor->setComponent(new TransformComponent(actor));
		std::cout << "Component(s) added." << std::endl;
		return actor;
	}

	void EntityManager::setComponent(Entity* ent, Component* comp)
	{
		ent->setComponent(comp);
	}
}
