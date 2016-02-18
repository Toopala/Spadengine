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

		void update(); // Every component derived from the Component base class needs to include this method;
	private:
		int counter; // Debug variables
		sge::math::vec2 paska;
		
	};

}
