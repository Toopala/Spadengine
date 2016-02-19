#include "Game/TestComponent.h"
#include <iostream>

namespace sge
{

	TestComponent::TestComponent(Entity* ent) : Component(ent)
	{
		std::cout << "TestComponent created." << std::endl;
		position = { 0.0f, 0.0f };
		
	}


	TestComponent::~TestComponent()
	{
	}

	void TestComponent::update()
	{
		std::cout << "Parent: "<< getParent() << "\nX: " << position.x << "\nY: " << position.y << "\n\n";
	} // System testing, just outputs data to the console if accessed.

	sge::math::vec2& TestComponent::getPosition()
	{
		return position;
	}

	void TestComponent::setPosition(const sge::math::vec2& newPos)
	{
		position.x = newPos.x;
		position.y = newPos.y;
	}

}