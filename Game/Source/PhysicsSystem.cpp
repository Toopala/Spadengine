#include "Game/PhysicsSystem.h"

namespace sge
{
	PhysicsSystem::PhysicsSystem() : System()
	{
		/*solver = new btSequentialImpulseConstraintSolver();
		collisionConfiguration = new btDefaultCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(collisionConfiguration);
		broadphase = new btDbvtBroadphase();


		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);*/
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