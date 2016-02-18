#pragma once
#include "TestComponent.h"
#include <vector>

namespace sge
{
	class TestSystem // System for testing system-based updating of components
	{
	public:
		TestSystem();
		~TestSystem();

		void update();
		void addComponent(TestComponent* comp);


	private:
		std::vector<TestComponent*> comps;
	};

}