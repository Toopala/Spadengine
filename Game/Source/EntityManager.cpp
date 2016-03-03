#include "Game/EntityManager.h"
#include <iostream>
#include <typeinfo>
namespace sge
{
	EntityManager::EntityManager()
	{
	}


	EntityManager::~EntityManager()
	{
	}

	Entity* EntityManager::createEntity() // Creates and returns an empty entity.
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

	void EntityManager::setManager(SystemManager* sysMgr)
	{
		sysManager = sysMgr;
	}

	void EntityManager::setComponent(Entity* ent, Component* comp)
	{
		ent->setComponent(comp);
		sysManager->addToSystem(comp);
	}
}
