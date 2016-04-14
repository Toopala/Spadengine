#pragma once

#include <vector>

#include "Game/Entity.h"
#include "Core/Math.h"

namespace sge
{
	class EntityManager
	{
	public:
		/** \brief Creates a transformable Entity.
		*
		* Creates an Entity that only contains a transform component.
		* \return Pointer to a default constructed Entity.
		*/
		Entity* createEntity();
	private:
		std::vector<Entity*> entities;
	};
}