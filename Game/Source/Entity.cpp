#include "Game/Entity.h"
#include "Core/Assert.h"
#include <iostream>

namespace sge
{
	Entity::Entity()
	{
		std::cout << "Entity created." << std::endl;
	}


	Entity::~Entity()
	{
	}

	void Entity::setComponent(Component* comp)
	{
		SGE_ASSERT(comp != nullptr); 

		components.push_back(comp); // Add a component to the entity's component vector
	}
}