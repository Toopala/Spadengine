#include "Game/TestSystem.h"

namespace sge
{

	TestSystem::TestSystem()
	{
	}


	TestSystem::~TestSystem()
	{
	}

	void TestSystem::update() // Call update on all components governed by this system
	{
		system("cls");
		for (int i = 0; i < comps.size(); i++)
		{
			comps[i]->update(); // Uses the virtual update that all components have
		}
	}

	void TestSystem::addComponent(TestComponent* comp)
	{
		comps.push_back(comp); // Adds a component to the system's container
	}

}