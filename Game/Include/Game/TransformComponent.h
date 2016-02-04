#pragma once
#include "Game/Component.h"

namespace sge
{

	class TransformComponent : public Component
	{
	public:
		TransformComponent(Entity* ent);
		~TransformComponent();
	};

}