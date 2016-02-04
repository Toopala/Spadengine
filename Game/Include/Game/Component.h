#pragma once


namespace sge
{
	class Entity;

	class Component
	{
	public:
		Component(Entity* ent);
		~Component();

		Entity* getParent()
		{
			return parent;
		}

	private:
		Entity* parent;
		
	};
}

