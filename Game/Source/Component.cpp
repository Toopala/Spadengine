#include "Game/Component.h"
#include "Core/Assert.h"

namespace sge
{
	Component::Component(Entity* ent) : parent(ent)
	{
		SGE_ASSERT(ent != nullptr); // Ensure that we have an entity to work with
	}


	Component::~Component()
	{
	}

	

}
