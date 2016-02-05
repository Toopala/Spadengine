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

