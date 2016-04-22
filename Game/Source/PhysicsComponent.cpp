#include "Game/PhysicsComponent.h"

namespace sge
{
	PhysicsComponent::PhysicsComponent(Entity* ent) : Component(ent)
	{
	//, collider(0), shape(0)
	}


	PhysicsComponent::~PhysicsComponent()
	{
	}

	void PhysicsComponent::update()
	{

	}

	/*void PhysicsComponent::setCollider(btCollisionObject* colliderType)
	{
		collider = colliderType;
	}

	void PhysicsComponent::setShape(btCollisionShape* objectShape)
	{
		shape = objectShape;
	}

	void PhysicsComponent::setMotionState(const btQuaternion &q, const btVector3 &c = btVector3(btScalar(0), btScalar(0), btScalar(0)))
	{
		motionState = new btDefaultMotionState(btTransform(q, c));
			
	}*/
}
