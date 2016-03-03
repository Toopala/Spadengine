#pragma once
#include "Game/Entity.h"
#include "TransformComponent.h"
#include "Game/SystemManager.h"

namespace sge
{
	class EntityManager
	{
	public:
		EntityManager();
		~EntityManager();

		Entity* createEntity(); // Creates a basic entity
		Entity* createActor(); // Example factory method

		void setComponent(Entity* ent, Component* comp); // Wraps a method, explained in cpp
		void setManager(SystemManager* sysMgr);

		template<class T> // Another wrapped function to remove a certain type of component from an entity
		void removeComponent(Entity& ent)
		{
			ent.removeComponent<T>();
		};

	private:

		SystemManager* sysManager;
	};
}

