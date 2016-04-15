#pragma once
#include <unordered_map>
#include "Game/System.h"
#include "Game/Component.h"

namespace sge
{
	class SystemManager
	{
	public:
		using Systems = std::unordered_map<size_t, System*>;
		
		/** \brief Adds a Component to a System.
		*
		* Uses the map to find the correct System for the Component and adds it
		* into that System.
		* \param Component* comp : Pointer to a type of Component.
		*/
		void addComponent(Component* comp);

		/** \brief Adds a System and a Component to a map.
		*
		* Pairs and adds a System and a Component to the systems map.
		* \param System* system : Pointer to a System.
		* \param size_t type : Type of Component.
		*/
		void addSystem(System* system, size_t type);

		/** \brief Updates all Systems.
		*
		* Calls the update functions of all Systems.
		*/
		void updateSystems();

	private:
		Systems systems; /**< Map used to find what Component goes to which System. */
	};
}

