#pragma once
#include "Game/System.h"
#include "Game/PhysicsComponent.h"
#include <vector>

namespace sge
{
	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem();
		~PhysicsSystem();

		void update();
		void addComponent(Component* comp);

	private:
		std::vector<PhysicsComponent*> comps;
	};
}
