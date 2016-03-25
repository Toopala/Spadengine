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

		void update();
	private:
		math::vec3 position;
		math::vec3 scale;
		float rotation;
	};

}