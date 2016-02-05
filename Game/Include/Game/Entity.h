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

			for (int i = 0; i < components.size(); i++)
			{
				foundIt = dynamic_cast<T*>(components[i]);
				
				if (foundIt)
					break;
			}

			return foundIt;
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
		}
		
		void setComponent(Component* comp);

	private:
		std::vector<Component*> components;
	};
}