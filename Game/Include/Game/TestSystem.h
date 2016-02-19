#pragma once
#include "TestComponent.h"
#include "InputComponent.h"
#include <vector>

namespace sge
{
	class TestSystem // System for testing system-based updating of components
	{
	public:
		TestSystem();
		~TestSystem();

		void update();
		void addTestComponent(TestComponent* comp);
		void addInputComponent(InputComponent* comp);

	private:
		std::vector<TestComponent*> comps1;
		std::vector<InputComponent*> comps2;
	};

}