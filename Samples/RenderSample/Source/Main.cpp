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
#include "Game/SpriteComponent.h"
#include "Game/TransformComponent.h"
#include "Game/ComponentFactory.h"
#include "Game/RenderSystem.h"

// TODO global is no no. :(
sge::EntityManager* entityManager;
sge::ComponentFactory<sge::TransformComponent>* transformFactory;
sge::ComponentFactory<sge::SpriteComponent>* spriteFactory;
sge::GraphicsDevice* device;

sge::math::mat4 VP = sge::math::ortho(0.0f, 1280.0f, 720.0f, 0.0f);
sge::Viewport viewport = { 0, 0, 1280, 720 };

sge::Entity* createSprite(sge::Texture* texture, const sge::math::vec3& position, const sge::math::vec3& scale, const sge::math::vec4& color, float rotation = 0)
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

    return entity;
}

int main(int argc, char** argv)
{
	// Init
	SDL_Init(SDL_INIT_VIDEO);

	sge::Window window("Spade Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720);
	sge::RenderSystem renderer(window);
    renderer.init();
    
    device = renderer.getDevice();

    sge::Handle<sge::TextureResource> textureResource = sge::ResourceManager::getMgr().load<sge::TextureResource>("../Assets/spade.png");
    sge::Handle<sge::TextureResource> textureResource2 = sge::ResourceManager::getMgr().load<sge::TextureResource>("../Assets/spade2.png");

    sge::Texture* texture = device->createTexture(
        textureResource.getResource<sge::TextureResource>()->getSize().x, 
        textureResource.getResource<sge::TextureResource>()->getSize().y,
        textureResource.getResource<sge::TextureResource>()->getData());

    sge::Texture* texture2 = device->createTexture(
        textureResource2.getResource<sge::TextureResource>()->getSize().x,
        textureResource2.getResource<sge::TextureResource>()->getSize().y,
        textureResource2.getResource<sge::TextureResource>()->getData());


    sge::ResourceManager::getMgr().printResources();

    transformFactory = new sge::ComponentFactory<sge::TransformComponent>();
    spriteFactory = new sge::ComponentFactory<sge::SpriteComponent>();
    entityManager = new sge::EntityManager();

	// TODO load layout description from external file (shader.reflect).

	// TODO move shader, pipeline, buffer and texture creation to somewhere else.
	// PLAN CAREFULLY.

	// What if render material had pipeline? And using material would bind it.


    device->bindViewport(&viewport);

	// TODO plan a simple (and smart) way to generate these commands.
	// Maybe we could generate it directly from renderdata?
	// These draw the triangle described in the vertex buffer to a given destination.

	// RenderData has all the data needed for rendering (d'oh). Vertex data, index data, matrices etc.
	// But we also have to somehow pass required shaders to renderer. Should this be done via material struct?
	// Shaders should be delivered so that you can sort the queue by them. This would optimize the drawing a bit
	// because all the data using the same shaders would be drawn in a one pass (?). This applies only to opaque
	// data, since transparent data should be sorted by their depth.

    auto sprite = createSprite(texture, { 512.0f, 256.0f, 1.0f }, { 192.0f, 256.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 0.6f });
    auto sprite2 = createSprite(texture2, { 256.0f, 256.0f, 0.0f }, { 256.0f, 256.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f });
	
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
        alpha += 0.001f;
        sprite->getComponent<sge::TransformComponent>()->setAngle(-alpha);
        sprite2->getComponent<sge::TransformComponent>()->setAngle(alpha);

		// Rendering
		renderer.begin();

		renderer.end();
        renderer.present();
        renderer.clear();
	}

	// Deinit

    device->deleteTexture(texture);
    device->deleteTexture(texture2);

    delete entityManager;
    delete transformFactory;
    delete spriteFactory;

	renderer.deinit();

	SDL_Quit();

	return 0;
}