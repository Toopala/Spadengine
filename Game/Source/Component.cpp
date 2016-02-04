#include "Game/Component.h"
#include <assert.h>


namespace sge
{
	Component::Component(Entity* ent) : parent(ent)
	{
		assert(ent != nullptr);
	}


	Component::~Component()
	{
	}

	

}