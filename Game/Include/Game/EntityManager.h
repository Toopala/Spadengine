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
		
		template<class T>
		void removeComponent(Entity& ent)
		{
			ent.removeComponent<T>();
		};
		
	};
}


// http://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2011/06/6-1-2010.pdf