#pragma once
#include "Game/Component.h"

namespace sge
{
	class System
	{
	public:
		System();
		virtual ~System();
		
		/** \brief Pure virtual function for Component addition
		*
		* Overwritten by systems, used by all systems to add a type of Component
		* to a matching container.
		* \param
		*/
		virtual void addComponent(Component* comp) = 0;
	};
}
