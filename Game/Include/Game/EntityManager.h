#pragma once
#include "Game/Entity.h"
#include "TransformComponent.h"


namespace sge
{
	class EntityManager
	{
	public:
		EntityManager();
		~EntityManager();

		Entity* createEntity();
		Entity* createActor();

		void setComponent(Entity* ent, Component* comp);

	};
}


//#include "Game/EntityManager.h"
//#include "Game/TransformComponent.h"

//sge::EntityManager* EManager = new sge::EntityManager();

//sge::Entity* player = EManager->createEntity();

//EManager->setComponent(player, new sge::TransformComponent(player));