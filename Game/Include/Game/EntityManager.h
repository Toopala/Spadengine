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
		* Creates an empty Entity and adds it to the manager's container.
		* \return Pointer to an empty Entity.
		*/
		Entity* createEntity();

		std::vector<Entity*>& getEntities()
		{
			return entities;
		}
	private:
		std::vector<Entity*> entities; /**< Vector of Entity pointers. */
	};
}
