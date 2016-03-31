#pragma once
#include "Game/Component.h"
#include "Core/Math.h"

namespace sge
{

	class TransformComponent : public Component
	{
	public:
		TransformComponent(Entity* ent);
		~TransformComponent();

		// TODO do we need update??????
		void update() {}

		void setPosition(const math::vec3& p)
		{
			position = p;
		}

		void setScale(const math::vec3& s)
		{
			scale = s;
		}

		void setRotationVector(const math::vec3& rv)
		{
			rotationVector = rv;
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

		float getAngle()
		{
			return angle;
		}

		const math::mat4 getMatrix()
		{
			return
				math::translate(math::mat4(1.0f), position) *
				math::rotate(math::mat4(1.0f), angle, rotationVector) *
				math::scale(math::mat4(1.0f), scale);
		}

	private:
		math::vec3 position;
		math::vec3 scale;
		math::vec3 rotationVector;
		float angle;
	};

}