#include "Game/EntityManager.h"
#include "Game/SystemManager.h"
#include "Game/SpriteRenderingSystem.h"

#include "Game/Component.h"
#include "Game/TransformComponent.h"
#include "Game/SpriteComponent.h"
#include "Game/InputComponent.h"
#include "Game/PhysicsComponent.h"

#include "Renderer/Texture.h"
#include "Renderer/Renderer.h"

#include <iostream>
#include <typeinfo>

namespace sge
{
	EntityManager::EntityManager(Renderer* renderer)
	{
		sysManager = new sge::SystemManager();
		sysManager->init(renderer);
		spritSys = sysManager->getSpriteSystem();
	}

	EntityManager::~EntityManager()
	{
		delete sysManager;
	}

	Entity* EntityManager::createEntity()
	{
		Entity *ent = new Entity();
		ent->setComponent(new sge::TransformComponent(ent));
		return ent;
	}

	Entity* EntityManager::createActor(sge::Texture* texture, const sge::math::vec4& color, const sge::math::vec3& position, const sge::math::vec3& scale, float rotation = 0)
	{
		Entity* ent = new Entity();

		sge::TransformComponent* tfComp = new sge::TransformComponent(ent, position, scale, rotation);
		sge::SpriteComponent* spComp = new sge::SpriteComponent(ent, spritSys, texture, color);

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

	void EntityManager::setComponent(Entity* ent, Component* comp)
	{
		ent->setComponent(comp);
		sysManager->addToSystem(comp);
	}

	void EntityManager::updateSystems()
	{
		sysManager->updateSystems();
	}
}
