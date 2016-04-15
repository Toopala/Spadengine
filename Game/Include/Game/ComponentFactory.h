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

		/** \brief Creates a component.
		*
		* Creates a Component of type T, adds it to the factory's container
		* and returns a pointer to it.
		* \param Entity* entity : Pointer to an Entity
		* \return Pointer to a type of Component.
		*/
		T* create(Entity* entity)
		{
			T* component = allocator.create<T>(entity);
            entity->setComponent(component);
			components.push_back(component);
			return component;
		}

		/** \brief Removes a component.
		*
		* Removes a component of type T.
		* \param T* component : Pointer to a type of Component
		*/
		void remove(T* component)
		{
			allocator.destroy<T>(component);
			components.erase(std::remove(components.begin(), components.end(), component), components.end());
		}

	private:
		std::vector<T*> components; /**< Vector of Component pointers */
	};
}