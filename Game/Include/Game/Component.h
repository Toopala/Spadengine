#pragma once
#include <string>

namespace sge
{
	class Entity;

	class Component
	{
	public:
		Component(Entity* ent);
		
		virtual ~Component();
		virtual void update() = 0; // Systems use this function to update their respective components

		Entity* getParent() // Returns the "owning" entity of the component
		{
			return parent;
		}

	private:
		Entity* parent;
		static std::string id;
	};
}

