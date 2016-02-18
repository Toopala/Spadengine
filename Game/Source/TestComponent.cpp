#include "Game/TestComponent.h"
#include <iostream>

namespace sge
{

	TestComponent::TestComponent(Entity* ent) : Component(ent)
	{
		std::cout << "TestComponent created." << std::endl;
	}


	TestComponent::~TestComponent()
	{
	}

	void TestComponent::update()
	{
		counter++;
		std::cout << "Updated TestComponent Owner: " << getParent()  << " Counter: " << counter << std::endl;
	} // System testing, just outputs data to the console if accessed.

}