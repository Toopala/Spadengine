#pragma once
#include "Component.h"

namespace sge
{
	class PhysicsComponent : public Component
	{
	public:
		PhysicsComponent(Entity* ent);
		~PhysicsComponent();

		void update();
	private:
		Entity* parent;
	};
}

