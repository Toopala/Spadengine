#pragma once
#include "Game/Component.h"
#include "Core/Math.h"

namespace sge
{

	class TransformComponent : public Component
	{
	public:
		TransformComponent(Entity* ent);

        void update() 
        {

        };

		void setPosition(const math::vec3& p)
		{
			position = p;
		}

        void addPosition(const math::vec3& p)
        {
            position += p;
        }

        void addAngle(float a)
        {
            angle += a;
        }

		void setScale(const math::vec3& s)
		{
			scale = s;
		}

		void setRotationVector(const math::vec3& rv)
		{
			rotationVector = rv;
		}

        void setFront(const math::vec3& f)
        {
            front = f;
        }

        void setUp(const math::vec3& u)
        {
            up = u;
        }

        void setLeft(const math::vec3& l)
        {
            left = l;
        }

		void setAngle(float a)
		{
			angle = a;
		}

		const math::vec3& getPosition()
		{
			return position;
		}

		const math::vec3& getScale()
		{
			return scale;
		}

		const math::vec3& getRotationVector()
		{
			return rotationVector;
		}

        const math::vec3& getFront()
        {
            return front;
        }

        const math::vec3& getUp()
        {
            return up;
        }

        const math::vec3& getLeft()
        {
            return left;
        }

		float getAngle()
		{
			return angle;
		}

		const math::mat4 getMatrix()
		{
			return
				math::translate(math::mat4(1.0f), position) *
				math::rotate(math::mat4(1.0f), angle, rotationVector) * // TODO lis‰‰ semmone et saa rotaation front/up/left-vektoreista!
				math::scale(math::mat4(1.0f), scale);
		}

        void lookAt(const math::vec3& target)
        {
            front = math::normalize(target - position);
        }

	private:
		math::vec3 position;
		math::vec3 scale;
		math::vec3 rotationVector;
        math::vec3 front;
        math::vec3 up;
        math::vec3 left;

		float angle;
	};

}