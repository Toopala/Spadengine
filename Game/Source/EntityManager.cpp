#include "Game/EntityManager.h"
#include "Core/Memory/PagePoolAllocator.h"

namespace sge
{
	Entity* EntityManager::createEntity()
	{
		Entity* entity = allocator.create<Entity>();
		entities.push_back(entity);
		return entity;
	}
}