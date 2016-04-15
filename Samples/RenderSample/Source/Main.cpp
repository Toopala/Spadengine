#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "SDL2/SDL.h"

#include "Core/Math.h"
#include "Core/Types.h"

#include "Renderer/Buffer.h"
#include "Renderer/Enumerations.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/Pipeline.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/RenderData.h"
#include "Renderer/RenderQueue.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/VertexLayout.h"
#include "Renderer/Viewport.h"
#include "Renderer/Window.h"

#include "Resources/ResourceManager.h"
#include "Resources/TextureResource.h"

#include "Game/EntityManager.h"
#include "Game/Entity.h"
#include "Game/SystemManager.h"
#include "Game/SpriteRenderingSystem.h"
#include "Game/SpriteComponent.h"
#include "Game/TransformComponent.h"
#include "Game/ComponentFactory.h"

// TODO global is no no. :(
sge::SystemManager* systemManager;
sge::EntityManager* entityManager;
sge::ComponentFactory<sge::TransformComponent>* transformFactory;
sge::ComponentFactory<sge::SpriteComponent>* spriteFactory;

sge::math::mat4 VP = sge::math::ortho(0.0f, 1280.0f, 720.0f, 0.0f);
sge::Viewport viewport = { 0, 0, 1280, 720 };

void createSprite(sge::Texture* texture, const sge::math::vec3& position, const sge::math::vec3& scale, const sge::math::vec4& color, float rotation = 0)
{
	// TODO kinda hax function. We need a proper way (a factory?) to create sprites.
    sge::Entity* entity = entityManager->createEntity();

    sge::TransformComponent* transform = transformFactory->create(entity);
    sge::SpriteComponent* sprite = spriteFactory->create(entity);

	transform->setPosition(position);
	transform->setScale(scale);
	transform->setRotationVector(sge::math::vec3(0.0f, 0.0f, 1.0f));
	transform->setAngle(rotation);
	
	sprite->setTexture(texture);
	sprite->setColor(color);

    systemManager->addComponent(sprite);
}

int main(int argc, char** argv)
{
	// Init
	SDL_Init(SDL_INIT_VIDEO);

	sge::Window window("Spade Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720);
	sge::Renderer renderer(window);
    renderer.init();

    sge::Handle<sge::TextureResource> textureResource = sge::ResourceManager::getMgr().load<sge::TextureResource>("../Assets/spade.png");

    sge::Texture* texture = renderer.getDevice()->createTexture(
        textureResource.getResource<sge::TextureResource>()->getSize().x, 
        textureResource.getResource<sge::TextureResource>()->getSize().y,
        textureResource.getResource<sge::TextureResource>()->getData());


    sge::ResourceManager::getMgr().printResources();

    transformFactory = new sge::ComponentFactory<sge::TransformComponent>();
    spriteFactory = new sge::ComponentFactory<sge::SpriteComponent>();
    systemManager = new sge::SystemManager();
    entityManager = new sge::EntityManager();

	// TODO load layout description from external file (shader.reflect).

	// TODO move shader, pipeline, buffer and texture creation to somewhere else.
	// PLAN CAREFULLY.

	// What if render material had pipeline? And using material would bind it.


	renderer.getDevice()->bindViewport(&viewport);

	// TODO plan a simple (and smart) way to generate these commands.
	// Maybe we could generate it directly from renderdata?
	// These draw the triangle described in the vertex buffer to a given destination.

	// RenderData has all the data needed for rendering (d'oh). Vertex data, index data, matrices etc.
	// But we also have to somehow pass required shaders to renderer. Should this be done via material struct?
	// Shaders should be delivered so that you can sort the queue by them. This would optimize the drawing a bit
	// because all the data using the same shaders would be drawn in a one pass (?). This applies only to opaque
	// data, since transparent data should be sorted by their depth.

    sge::SpriteRenderingSystem* spriteRenderer = new sge::SpriteRenderingSystem(&renderer);
    systemManager->addSystem(spriteRenderer, typeid(sge::SpriteComponent).hash_code());
    spriteRenderer->setVP(VP);

    createSprite(texture, { 512.0f, 256.0f, 1.0f }, { 128.0f, 256.0f, 1.0f }, { 1.0f, 0.0f, 0.3f, 0.6f });
    createSprite(texture, { 256.0f, 256.0f, 0.0f }, { 256.0f, 256.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f });
	
	// Loop
	SDL_Event event;
	bool running = true;
	float alpha = 0.0f;

	while (running)
	{
		// Events
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
				break;
			}
		}

		// Update

		// Rendering
		renderer.begin();

        systemManager->updateSystems();

		renderer.end();
	}

	// Deinit

    renderer.getDevice()->deleteTexture(texture);

    delete spriteRenderer;
    delete systemManager;
    delete entityManager;
    delete transformFactory;
    delete spriteFactory;

	renderer.deinit();

	SDL_Quit();

	return 0;
}