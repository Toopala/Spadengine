#pragma once
#include "Game/System.h"
#include "Game/Component.h"
#include "Game/TestComponent.h"
#include "Game/InputComponent.h"
#include <vector>

namespace sge
{
	class TestSystem : public System // System for testing system-based updating of components
	{
	public:
		TestSystem();
		~TestSystem();

		void update();
	
		void addComponent(Component* comp);

	private:
		std::vector<TestComponent*> comps1;
		std::vector<InputComponent*> comps2;
	};

}