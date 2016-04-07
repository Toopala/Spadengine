#include "Game/EntityManager.h"
#include "Game/SystemManager.h"

#include "Game/Component.h"
#include "Game/TransformComponent.h"
#include "Game/SpriteComponent.h"
#include "Game/InputComponent.h"
#include "Game/PhysicsComponent.h"

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

	Entity* EntityManager::createEntity()
	{
		Entity *ent = new Entity();
		ent->setComponent(new sge::TransformComponent(ent));
		return ent;
	}

	Entity* EntityManager::createActor()
	{
		Entity* ent = new Entity();

		sge::TransformComponent* tfComp = new sge::TransformComponent(ent);
		sge::SpriteComponent* spComp = new sge::SpriteComponent(ent);
		ent->setComponent(tfComp);
		ent->setComponent(spComp);

		sysManager->addToSystem(tfComp);
		sysManager->addToSystem(spComp);

		return ent;
	}

	Entity* EntityManager::createPhysicsActor()
	{
		Entity *ent = new Entity();
		ent->setComponent(new sge::TransformComponent(ent));
		ent->setComponent(new sge::SpriteComponent(ent));
		ent->setComponent(new sge::PhysicsComponent(ent));
		return ent;
	}

	Entity* EntityManager::createPlayer()
	{
		Entity *ent = new Entity();
		ent->setComponent(new sge::TransformComponent(ent));
		ent->setComponent(new sge::SpriteComponent(ent));
		ent->setComponent(new sge::InputComponent(ent));
		ent->setComponent(new sge::PhysicsComponent(ent));
		return ent;
	}

	void EntityManager::setSysManager(SystemManager* sysMgr)
	{
		sysManager = sysMgr;
	}

	void EntityManager::setComponent(Entity* ent, Component* comp)
	{
		ent->setComponent(comp);
		sysManager->addToSystem(comp);
	}
}
