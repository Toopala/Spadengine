#pragma once

#include "Game/Component.h"
#include "Core/Math.h"

namespace sge
{
	class TransformComponent;

	class LightComponent : public Component
	{
	public:
		LightComponent(Entity* entity);
		virtual ~LightComponent();
		virtual void update();

	protected:
		TransformComponent* transform;
	};
}