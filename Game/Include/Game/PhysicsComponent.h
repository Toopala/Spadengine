#pragma once
#include "Component.h"

#include <Bullet/btBulletDynamicsCommon.h>


namespace sge
{
	class PhysicsComponent : public Component
	{
	public:
		PhysicsComponent(Entity* ent);
		~PhysicsComponent();

		void update();

		/*void setBody(btRigidBody* bodyType)
		{
			body = bodyType;
		}*/
		
		/*void setShape(btCollisionShape* objectShape)
		{
			shape = objectShape;
		}*/
		
		template <typename T>
		T* getBody()
		{
			return dynamic_cast<T*>(body);
		}

		template <typename T>
		T* getShape()
		{
			return dynamic_cast<T*>(shape);
		}

		/*btCollisionObject* getBody()
		{
			return body;
		}*/

		btRigidBody* createBody(btCollisionShape* objectShape, btQuaternion& rotation, btVector3& location, btScalar& objectMass, btVector3& inertia);


	private:
		btTransform trans;
		btRigidBody* body;
		btCollisionShape* shape;
	};
}

/*

btJokuVittu* createBody(btquaternion, btvector3, btscalar, jokusimpukka shape, btVector3 siika);

btDefaultMotionState* motistate
= new btdefaultmotionstate(btTransfrom(btquaternion, btvector3);
btScalar mass = jotain;
btVector3 fallInertia = siika;
btrigidbody::btrigidbodyconstructioninfo consinfo(mass, motistate, shape, inertia);
consinfo.m_restitution = restitution;
consinfo.m_friction = friction;
body = new btRigidBody(consinfo);
body->setActivationState(DISABLE_DEACTIVATION);
dynamicsWorld->addRigidBody(body);

return body;
*/
