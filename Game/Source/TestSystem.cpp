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
		for (size_t i = 0; i < comps1.size(); i++)
		{
			comps1[i]->update(); // Uses the virtual update that all components have
		}
		for (size_t i = 0; i < comps2.size(); i++)
		{
			comps2[i]->update(); // Uses the virtual update that all components have
		}
	}

	void TestSystem::addTestComponent(TestComponent* comp)
	{
		comps1.push_back(comp); // Adds a component to the system's container
	}

	void TestSystem::addInputComponent(InputComponent* comp)
	{
		comps2.push_back(comp); // Adds a component to the system's container
	}

}