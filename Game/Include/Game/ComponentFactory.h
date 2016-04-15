#pragma once
#include <vector>
#include "Game/Entity.h"
#include "Core/Memory/PagePoolAllocator.h"


namespace sge
{
	template<typename T>
	class ComponentFactory
	{
	public:
		~ComponentFactory()
		{
			for (auto component : components)
			{
				allocator.destroy<T>(component);
			}
			components.clear();
		};

		T* create(Entity* entity)
		{
			T* component = allocator.create<T>(entity);
            entity->setComponent(component);
			components.push_back(component);
			return component;
		}

		void remove(T* component)
		{
			allocator.destroy<T>(component);
			components.erase(std::remove(components.begin(), components.end(), component), components.end());
		}

	private:
		std::vector<T*> components;
	};
}