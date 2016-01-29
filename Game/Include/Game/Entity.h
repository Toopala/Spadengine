#pragma once

#include "Game/Component.h"
#include <vector>

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
			for (int i = 0; i < components.size(); i++)
			{
				Component* c = components[i];
				if (typeid(T) == typeid(*c))
				{
					static_cast<T*>(c);
				}
				return nullptr;
			}
		}

	private:
		std::vector<Component*> components;
	};
}