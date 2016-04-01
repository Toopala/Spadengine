#pragma once

#include "Game/Entity.h"

namespace sge
{
	class SystemManager;
	class Component;

	class EntityManager
	{
	public:
		EntityManager();
		~EntityManager();

		/** \brief Creates an empty Entity.
		*
		* Empty default constructed Entity.
		* \return Pointer to a default constructed Entity.
		*/
		Entity* createEntity();

		/** \brief Entity factory function.
		*
		* Creates a new Entity and gives it certain components.
		* \return Pointer to created Entity.
		*/
		Entity* createActor();

		/** \brief Function for adding Components.
		*
		* Adds a Component to the Entity.
		* \param Entity* ent : Pointer to Entity 
		* \param Component* comp : Pointer to a type of Component.
		*/
		void setComponent(Entity* ent, Component* comp);
		
		/** \brief Setter Function for SystemManager
		*
		* Sets class sysManager pointer to received pointer.
		* \param SystemManager* sysMgr : Pointer to SystemManager.
		*/
		void setSysManager(SystemManager* sysMgr);

		/** \brief Function for removing a Component
		*
		* Wraps the removeComponent method of the Entity class.
		* Removes any type of component from an Entity.
		* \param Entity& ent : Reference to the Entity we want to remove Component T from.
		*/
		template<class T>
		void removeComponent(Entity& ent)
		{
			ent.removeComponent<T>();
		};

	private:

		SystemManager* sysManager; /**< SystemManager pointer */
	};
}