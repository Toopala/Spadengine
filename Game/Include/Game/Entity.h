#pragma once

#include "Game/Component.h"
#include <vector>
#include <algorithm>

namespace sge
{
	class Entity
	{
	public:
		Entity();
		~Entity();

		template<class T>
		T* getComponent()
		{
			T* foundIt = nullptr;

			for (int i = 0; i < components.size(); i++) // Iterate through the vector
			{
				foundIt = dynamic_cast<T*>(components[i]); // Casts the found component to its original type
				
				if (foundIt) // Exit the loop, entities generally only have one component of a type
					break;
			}

			return foundIt; // Return the found component
		}
		
		template<class T>
		void removeComponent() 
		{
			components.erase(std::remove_if(components.begin(), components.end(),
				[&](Component* component) 
				{
					return getComponent<T>();
				}),
				components.end());
		} // Finds and removes the component from the entity's vector
		
		void setComponent(Component* comp); // Called by entitymanager 

	private:
		std::vector<Component*> components; // All the components the entity has are stored in this vector
	};
}