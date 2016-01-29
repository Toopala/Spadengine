#pragma once

namespace sge
{
	class Entity;

	class Component
	{
	public:
		Component();
		~Component();
		Entity* getParent()
		{
			return parent;
		}
	private:
		Entity* parent;
		
	};
}

