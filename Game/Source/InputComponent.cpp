#include "Game/InputComponent.h"
#include "Game/Entity.h"
#include "Game/TestComponent.h"

namespace sge
{
	InputComponent::InputComponent(Entity* ent) : Component(ent)
	{
	}


	InputComponent::~InputComponent()
	{
	}

	void InputComponent::update()
	{
		//while (SDL_PollEvent(&event))
		//{
		//	switch (event.type)
		//	{
		//	case SDL_KEYDOWN: // Uses TestComponent's setter function to change values
		//		
		//		sge::math::vec2 tempPos = getParent()->getComponent<TestComponent>()->getPosition();
		//		
		//		if (event.key.keysym.sym == SDLK_LEFT)
		//		{ 
		//			getParent()->getComponent<TestComponent>()->setPosition(sge::math::vec2(--tempPos.x,tempPos.y));
		//		}
		//		if (event.key.keysym.sym == SDLK_RIGHT)
		//		{
		//			getParent()->getComponent<TestComponent>()->setPosition(sge::math::vec2(++tempPos.x, tempPos.y));
		//		}
		//		if (event.key.keysym.sym == SDLK_UP)
		//		{
		//			getParent()->getComponent<TestComponent>()->setPosition(sge::math::vec2(tempPos.x, ++tempPos.y));
		//		}
		//		if (event.key.keysym.sym == SDLK_DOWN)
		//		{
		//			getParent()->getComponent<TestComponent>()->setPosition(sge::math::vec2(tempPos.x, --tempPos.y));
		//		}
		//		break;
		//	}
		//}
	}
}