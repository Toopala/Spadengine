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
		* \param Pointer to Entity, Pointer to a type of Component.
		*/
		void setComponent(Entity* ent, Component* comp);
		
		/** \brief Setter Function for SystemManager
		*
		* Sets class sysManager pointer to received pointer.
		* \param Pointer to SystemManager.
		*/
		void setSysManager(SystemManager* sysMgr);

		/** \brief Function for removing a Component
		*
		* Wraps the removeComponent method of the Entity class.
		* Removes any type of component from an Entity.
		* \param Pointer to type of Component as template parameter, Pointer to Entity
		*/
		template<class T>
		void removeComponent(Entity& ent)
		{
			ent.removeComponent<T>();
		};

	private:

		SystemManager* sysManager; ///< SystemManager pointer
	};
}