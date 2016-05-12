#pragma once

#include <vector>
#include <string>
#include <algorithm>

namespace sge
{
	class Component;
	class Entity
	{
	public:
        Entity() : tag("generic")
		{
         
		}
		/** \brief Getter function for Components.
		*
		* Gets a Component pointer of the called type T.
		* Iterates through the Entity's Component vector dynamic_casting Components to the wanted type.
		* The loop terminates if a Component is successfully cast.
		* Returns a nullpointer if a suitable Component isn't found.
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
		* Removes Component T from the Entity's Component vector by performing satanic rituals.
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
		* \param Component* comp : Pointer to a type of Component.
		*/
		void setComponent(Component* comp); 

        void setTag(const std::string& tag)
        {
            this->tag = tag;
        }

        const std::string& getTag()
        {
            return tag;
        }

	private:
        std::string tag;
		std::vector<Component*> components; /**< Vector of Component pointers */
	};
}

