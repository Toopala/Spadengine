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

		/** \brief Pure virtual function for updating.
		*
		* Updates the Component.
		*/
		virtual void update() = 0;

		/** \brief Getter function for the Component's parent.
		*
		* Gets the parent or "owner" Entity of the Component and returns the pointer to it.
		* \return Pointer to an Entity.
		*/
		Entity* getParent() 
		{
			return parent;
		}

	private:
		Entity* parent; /**< Pointer to the parent Entity. */
	};
}
