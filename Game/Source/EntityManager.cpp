#include "Game/EntityManager.h"
#include "Game/SystemManager.h"

#include "Game/Component.h"

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
		
		return actor;
	}

	void EntityManager::setSysManager(SystemManager* sysMgr)
	{
		sysManager = sysMgr;
	}

	/*!Brief
	*	Brief
	*
	*	Detailed
	*/	

	void EntityManager::setComponent(Entity* ent, Component* comp)
	{
		ent->setComponent(comp);
		sysManager->addToSystem(comp);
	}
}
