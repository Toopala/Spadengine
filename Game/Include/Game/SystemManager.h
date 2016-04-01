#pragma once
#include <unordered_map>
#include "Game/System.h"
#include "Game/Component.h"
#include "Game/TestSystem.h"
#include "Game/TestComponent.h"
#include "Core/Memory/PagePoolAllocator.h"

namespace sge
{

	class SystemManager
	{
	public:
		SystemManager();
		~SystemManager();

		using Systems = std::unordered_map<size_t, System*>;

		/** \brief Initialization function.
		*
		* Initializes all Systems governed by the manager.
		* Creates a map which is used for automatic component addition.
		* Pairing a hashcode of a Component typeid with a System causes all
		* Components of that type to be added to that System's container.
		*/
		void init();
		
		/** \brief Adds a Component to a System.
		*
		* Uses the map to find the correct System for the Component and adds it
		* into that System.
		* \param Component* comp Pointer to a type of Component.
		*/
		void addToSystem(Component* comp);

		/** \brief Updates all Systems.
		*
		* Calls the update functions of all Systems.
		*/
		void updateSystems();

	private:
		Systems systems; /**< Map used to find what Component goes to which System. */
		TestSystem* testSys; /**< Pointer to a TestSystem. */
	
	};
}

