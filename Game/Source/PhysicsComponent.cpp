#include "Game/PhysicsComponent.h"
#include "Game/Entity.h"
#include "Game/TransformComponent.h"
#include "Core/Assert.h"

namespace sge
{
	PhysicsComponent::PhysicsComponent(Entity* ent) : Component(ent)
	{	
	}


	PhysicsComponent::~PhysicsComponent()
	{
		/*delete body->getMotionState();
		delete body;*/ // May B
	}
	

	void PhysicsComponent::update()
	{
		if (getBody<btRigidBody>() != nullptr)
		{
			getBody<btRigidBody>()->getMotionState()->getWorldTransform(trans);

			SGE_ASSERT(getParent()->getComponent<TransformComponent>());
			getParent()->getComponent<TransformComponent>()->setPosition(sge::math::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
			getParent()->getComponent<TransformComponent>()->setAngle(trans.getRotation().getAngle());
			getParent()->getComponent<TransformComponent>()->setRotationVector(sge::math::vec3(trans.getRotation().getAxis().getX(), trans.getRotation().getAxis().getY(), trans.getRotation().getAxis().getZ()));
		}
	}

	btRigidBody* PhysicsComponent::createBody(btCollisionShape* objectShape, btQuaternion& rotation, btVector3& location, btScalar& objectMass, btVector3& inertia)
	{
		shape = objectShape;
		btDefaultMotionState* motiState
			= new btDefaultMotionState(btTransform(rotation, location));
		btScalar mass = objectMass;
		btVector3 fallInertia = inertia;
		shape->calculateLocalInertia(mass, fallInertia);
		btRigidBody::btRigidBodyConstructionInfo consInfo(mass, motiState, shape, fallInertia);
		body = new btRigidBody(consInfo);
		return body;
	}
}
