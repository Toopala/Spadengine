#include "Game/Entity.h"
#include <assert.h>
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
		assert(comp != nullptr && "Component pointer null."); // Ebin hax :D

		components.push_back(comp);
	}
}