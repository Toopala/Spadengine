#include "Game/PhysicsSystem.h"

namespace sge
{
	PhysicsSystem::PhysicsSystem() : System()
	{
	}


	PhysicsSystem::~PhysicsSystem()
	{
	}

	void PhysicsSystem::update()
	{
		for (size_t i = 0; i < comps.size(); i++)
		{
			comps[i]->update();
		}
	}

	void PhysicsSystem::addComponent(Component* comp)
	{
		comps.push_back(dynamic_cast<PhysicsComponent*>(comp));
	}
}