#pragma once
#include "Game/System.h"
#include "Game/Component.h"
#include "Game/TestComponent.h"
#include "Game/InputComponent.h"
#include <vector>

namespace sge
{
	class TestSystem : public System 
	{
	public:
		TestSystem();
		~TestSystem();

		/** \brief Function for updating the System.
		*
		* Iterates through the Component vector(s) calling each Component's update
		* method.
		* \return
		*/
		void update();

		/** \brief Function for adding Components.
		*
		* Casts the Component to the vector's type and adds it.
		* \param Pointer to a type of Component.
		*/
		void addComponent(Component* comp);

	private:
		std::vector<TestComponent*> comps1; ///< Vector containing TestComponent pointers. 
		std::vector<InputComponent*> comps2; ///< Vector containing InputComponent pointers.
	};

}