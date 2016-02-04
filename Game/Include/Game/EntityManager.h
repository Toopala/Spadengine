#pragma once
#include "Game/Entity.h"
#include "TransformComponent.h"


namespace sge
{
	enum class ComponentType
	{
		TRANSFORMCOMPONENT = 0,
		
	};


	class EntityManager
	{
	public:
		EntityManager();
		~EntityManager();

		Entity* createEntity();
		Entity* createActor();

		void setComponent(Entity* ent, ComponentType type);

	};
}

