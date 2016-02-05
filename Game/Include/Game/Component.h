#pragma once


namespace sge
{
	class Entity;

	class Component
	{
	public:
		Component(Entity* ent);
		virtual ~Component();

		Entity* getParent()
		{
			return parent;
		}

	private:
		Entity* parent;
		
	};
}

