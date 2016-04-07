#include "Game/TransformSystem.h"

namespace sge
{
	TransformSystem::TransformSystem() : System()
	{
	}


	TransformSystem::~TransformSystem()
	{
	}

	void TransformSystem::update()
	{
		for (size_t i = 0; i < comps.size(); i++)
		{
			comps[i]->update();
		}
	}


	void TransformSystem::addComponent(Component* comp)
	{
		comps.push_back(dynamic_cast<TransformComponent*>(comp));
	}

}
