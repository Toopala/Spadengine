#pragma once
#include "Game/System.h"
#include "Game/TransformComponent.h"
#include <vector>

namespace sge
{
	class TransformSystem : public System
	{
	public:
		TransformSystem();
		~TransformSystem();

		void update();
		void addComponent(Component* comp);

	private:
		std::vector<TransformComponent*> comps;
	};
}
