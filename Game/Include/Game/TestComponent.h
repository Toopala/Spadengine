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
		sge::math::vec2& getPosition();
		void setPosition(const sge::math::vec2& newPos);

	private:
		int counter; // Debug variables
		sge::math::vec2 position;
		
	};

}
