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
		
		/** \brief Getter function for Components.
		*
		* Gets a Component pointer of the called type.
		* Iterates through the Entity's Component vector dynamic_casting Components to the wanted type.
		* The loop terminates if a Component is successfully cast.
		* Returns a nullpointer if a suitable Component isn't found.
		* \param Pointer to a type of Component.
		* \return Component pointer of the desired type.
		*/
		template<class T>
		T* getComponent()
		{
			T* foundIt = nullptr;

			for (size_t i = 0; i < components.size(); i++)
			{
				foundIt = dynamic_cast<T*>(components[i]);
				
				if (foundIt)
					break;
			}

			return foundIt;
		}
		
		/** \brief Component Removal function.
		*
		* Removes a Component from the Entity's vector by performing satanic rituals.
		* \param Pointer to a type of Component
		*/
		template<class T>
		void removeComponent() 
		{
			components.erase(std::remove_if(components.begin(), components.end(),
				[&](Component* component) 
				{
					return getComponent<T>();
				}),
				components.end());
		}
		
		/** \brief Setter function for Components.
		*
		* Pushes a new Component to the back of the Component vector.
		* \param Pointer to a type of Component.
		*/
		void setComponent(Component* comp); 

	private:
		std::vector<Component*> components; ///< Vector of Component pointers
	};
}

