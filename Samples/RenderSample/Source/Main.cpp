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

#include "Game/SpriteComponent.h"
#include "Game/SpriteRenderingSystem.h"
#include "Game/TransformComponent.h"
#include "Game/Entity.h"

#include "Resources/ResourceManager.h"

// TODO global is no no. :(
std::vector<sge::Entity*> entities;
std::vector<sge::TransformComponent*> transforms;
std::vector<sge::SpriteComponent*> sprites;

sge::math::mat4 VP = sge::math::ortho(0.0f, 1280.0f, 720.0f, 0.0f);
sge::Viewport viewport = { 0, 0, 1280, 720 };

void createSprite(sge::SpriteRenderingSystem* system, const sge::math::vec3& position, const sge::math::vec3& scale, const sge::math::vec4& color, float rotation = 0)
{
	// TODO kinda hax function. We need a proper way (a factory?) to create sprites.
	sge::Entity* entity = new sge::Entity();

	sge::TransformComponent* transform = new sge::TransformComponent(entity);
	entity->setComponent(transform);

	sge::SpriteComponent* sprite = new sge::SpriteComponent(entity);
	entity->setComponent(sprite);

	transform->setPosition(position);
	transform->setScale(scale);
	transform->setRotationVector(sge::math::vec3(0.0f, 0.0f, 1.0f));
	transform->setAngle(rotation);
	
	sprite->setTexture(nullptr);
	sprite->setColor(color);
    sprite->setRenderingSystem(system);

	entities.push_back(entity);
	sprites.push_back(sprite);
	transforms.push_back(transform);

    system->addComponent(sprite);
}

int main(int argc, char** argv)
{
	// Init
	SDL_Init(SDL_INIT_VIDEO);

	sge::Window window("Spade Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720);
	sge::Renderer renderer(window);
    renderer.init();


    sge::ResourceManager::getMgr().printResources();

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

    sge::SpriteRenderingSystem spriteRenderingSystem(&renderer);
    spriteRenderingSystem.setVP(VP);

    createSprite(&spriteRenderingSystem, { 256.0f, 256.0f, 0.0f }, { 256.0f, 256.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f });
    createSprite(&spriteRenderingSystem, { 512.0f, 256.0f, 0.0f }, { 128.0f, 256.0f, 1.0f }, { 1.0f, 0.0f, 0.3f, 0.6f });
	
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
		// TODO add fixed timestep.
		alpha += 0.0001f;
		transforms.back()->setAngle(alpha);

		// Rendering
		renderer.begin();

        spriteRenderingSystem.update();

		renderer.end();
	}

	// Deinit

	for (auto sprite : sprites)
	{
		delete sprite;
	}

	for (auto transform : transforms)
	{
		delete transform;
	}

	for (auto entity : entities)
	{
		delete entity;
	}

	sprites.clear();
	transforms.clear();
	entities.clear();

	renderer.deinit();

	SDL_Quit();

	return 0;
}