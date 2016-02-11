#include "Game/Component.h"
#include <assert.h>
#include "Core\Assert.h"

namespace sge
{
	Component::Component(Entity* ent) : parent(ent)
	{
		assert(ent != nullptr);
		SGE_ASSERT(ent != nullptr);
	}


	Component::~Component()
	{
	}

	

}