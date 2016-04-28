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
}
