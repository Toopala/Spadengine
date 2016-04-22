#pragma once
#include "Component.h"

//#include <Bullet/btBulletDynamicsCommon.h>


namespace sge
{
	class PhysicsComponent : public Component
	{
	public:
		PhysicsComponent(Entity* ent);
		~PhysicsComponent();

		void update();
	/*	void setCollider(btCollisionObject* colliderType);
		void setShape(btCollisionShape* objectShape);
		void setMotionState(const btQuaternion &q, const btVector3 &c);*/
	private:
		/*Entity* parent;
		btCollisionObject* collider;
		btCollisionShape* shape;
		btDefaultMotionState* motionState;*/
	};
}

