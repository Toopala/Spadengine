#pragma once
#include "Game/System.h"
#include "Game/PhysicsComponent.h"
#include "Game/ComponentFactory.h"
#include <vector>

namespace sge
{
	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem();
		~PhysicsSystem();

		void update();
		void stepWorld(float deltaTime); // Could also change update to contain deltatime
		void addComponent(Component* comp);
		PhysicsComponent* createPhysicsComponent(Entity* ent);

		btDiscreteDynamicsWorld* getWorld()
		{
			return dynamicsWorld;
		}

		void addBody(btRigidBody* body)
		{
			dynamicsWorld->addRigidBody(body);
		}

	private:
		std::vector<PhysicsComponent*> comps;
		sge::ComponentFactory<PhysicsComponent> physFac;

		// Bullet init
		
		btDiscreteDynamicsWorld* dynamicsWorld;
		btSequentialImpulseConstraintSolver* solver;
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* broadphase;


	};
}
