#include "Game/EntityManager.h"
#include "Game/TransformComponent.h"
#include "Game/InputComponent.h"
#include "Game/TestComponent.h"
#include "Game/SystemManager.h"
#include "Game/ComponentFactory.h"
#include "SDL2/SDL.h"
#include "Core/Math.h"
#include "Renderer/Window.h"
#include "Renderer/Renderer.h"
#include "Renderer/GraphicsDevice.h"


int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO); // Initializing

	SDL_Event event;

	sge::Window window("The test to end all tests.", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600);
	sge::Renderer renderer(window);
	renderer.init();

	sge::ComponentFactory<sge::TransformComponent> transFactor;

	// Create EntityManager
	
	sge::EntityManager EManager;
	// Create an entity through the manager

	

	sge::Entity* player1 = EManager.createEntity();
	sge::Entity* player2 = EManager.createEntity();


	// Give Entities components by asking the factory of that specific component type.
	player1->setComponent(transFactor.create(player1));
	player1->setComponent(transFactor.create(player1));


	// Give the entity a certain component
	// Looooop

	bool running = true;
	while (running)
	{
		renderer.getDevice()->clear(0.5f, 0.0f, 0.7f, 1.0f);

		while (SDL_PollEvent(&event)) 
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					running = false;
				}
			}
		}

		renderer.getDevice()->swap();
	}

	// Testing component deletion
	renderer.deinit();

	return 0;
}