#include "Game/Entity.h"
#include <cassert>
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
		assert(comp != nullptr); 

		components.push_back(comp);
	}
}