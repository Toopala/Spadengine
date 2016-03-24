#include "Game/TestSystem.h"
#include <typeinfo>

namespace sge
{

	TestSystem::TestSystem() : System()
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

	void TestSystem::addComponent(Component* comp)
	{
		if (typeid(*comp) == typeid(TestComponent))
		{
			comps1.push_back(dynamic_cast<TestComponent*>(comp));
		}

		if (typeid(*comp) == typeid(InputComponent))
		{
			comps2.push_back(dynamic_cast<InputComponent*>(comp));
		}
	}

}
