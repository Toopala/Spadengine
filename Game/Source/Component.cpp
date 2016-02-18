#include "Game/Component.h"
#include "Core/Assert.h"

namespace sge
{
	Component::Component(Entity* ent) : parent(ent)
	{
		SGE_ASSERT(ent != nullptr);
	}


	Component::~Component()
	{
	}

	

}
