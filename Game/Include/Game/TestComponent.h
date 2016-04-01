#pragma once
#include "Game/Component.h"
#include "Core/Math.h"

namespace sge
{

	class TestComponent : public Component // Testing component used with testsystem
	{
	public:
		TestComponent(Entity* ent);
		~TestComponent();

		/** \brief Update function.
		*
		* Detailed
		* \return
		*/
		void update(); // Every component derived from the Component base class needs to include this method;

		/** \brief Position getter function.
		*
		* Gets the value stored in the position variable.
		* \return Value stored in the vec2 position variable.
		*/
		sge::math::vec2& getPosition();

		/** \brief Setter function
		*
		* \param const sge::mat::vec2& newPos : New position in vec2 form.
		* \return
		*/
		void setPosition(const sge::math::vec2& newPos);

	private:
		int counter; /**< Debug variable. */
		sge::math::vec2 position; /**< Position variable. */
		
	};

}

/** \brief
*
* Detailed
* \return
*/

///<