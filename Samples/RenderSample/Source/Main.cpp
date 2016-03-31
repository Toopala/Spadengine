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
#include "Game/TransformComponent.h"
#include "Game/Entity.h"

void loadTextShader(const std::string& path, std::vector<char>& data)
{
	std::ifstream file;

	file.open(path, std::ios::in);

	if (file.is_open())
	{
		std::cout << "SUCCESS: Opened shader " << path << std::endl;

		std::stringstream stream;
		std::string str;

		stream << file.rdbuf();

		str = stream.str();

		std::copy(str.begin(), str.end(), std::back_inserter(data));

		data.push_back('\0');
	}
	else
	{
		std::cout << "ERROR: Could not open shader: " << path << std::endl;
	}
}

void loadBinaryShader(const std::string& path, std::vector<char>& data)
{
	std::ifstream file;

	file.open(path, std::ios::in | std::ios::ate | std::ios::binary);

	if (file.is_open())
	{
		std::cout << "SUCCESS: Opened shader " << path << std::endl;
		data.resize(static_cast<size_t>(file.tellg()));

		file.seekg(0, std::ios::beg);
		file.read(data.data(), data.size());
		file.close();
	}
	else
	{
		std::cout << "ERROR: Could not open shader: " << path << std::endl;
	}
}

// TODO global is no no. :(
std::vector<sge::Entity*> entities;
std::vector<sge::TransformComponent*> transforms;
std::vector<sge::SpriteComponent*> sprites;

float vertexData[] = {
	0.0f, 1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,
};

struct UniformData
{
	sge::math::mat4 MVP;
	sge::math::vec4 color;
};

sge::math::mat4 VP = sge::math::ortho(0.0f, 1280.0f, 720.0f, 0.0f);
sge::Shader* vertexShader;
sge::Shader* pixelShader;
sge::Pipeline* pipeline;
sge::Viewport viewport = { 0, 0, 1280, 720 };
sge::Buffer* vertexBuffer;
sge::Buffer* uniformBuffer;

void createSprite(const sge::math::vec3& position, const sge::math::vec3& scale, const sge::math::vec4& color, float rotation = 0)
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
	
	sprite->setPipeline(pipeline);
	sprite->setTexture(nullptr);
	sprite->setUniformBuffer(uniformBuffer);
	sprite->setVertexBuffer(vertexBuffer);
	sprite->setVP(VP);
	sprite->setColor(color);

	entities.push_back(entity);
	sprites.push_back(sprite);
	transforms.push_back(transform);
}

int main(int argc, char** argv)
{
	// Init
	SDL_Init(SDL_INIT_VIDEO);

	sge::Window window("Spade Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720);
	sge::Renderer renderer(window);

	renderer.init();

	std::vector<char> pShaderData;
	std::vector<char> vShaderData;

#ifdef DIRECTX11
	loadBinaryShader("../../Shaders/Compiled/SimpleVertexShader.cso", vShaderData);
	loadBinaryShader("../../Shaders/Compiled/SimplePixelShader.cso", pShaderData);
#elif OPENGL4
	loadTextShader("../../Shaders/Compiled/SimpleVertexShader.glsl", vShaderData);
	loadTextShader("../../Shaders/Compiled/SimplePixelShader.glsl", pShaderData);
#endif

	// TODO load layout description from external file (shader.reflect).

	sge::VertexLayoutDescription vertexLayoutDescription = { 1,
	{
		{ 0, 3, sge::VertexSemantic::POSITION }
	}};

	// TODO move shader, pipeline, buffer and texture creation to somewhere else.
	// PLAN CAREFULLY.

	// What if render material had pipeline? And using material would bind it.

	vertexShader = renderer.getDevice().createShader(sge::ShaderType::VERTEX, vShaderData.data(), vShaderData.size());
	pixelShader = renderer.getDevice().createShader(sge::ShaderType::PIXEL, pShaderData.data(), pShaderData.size());
	pipeline = renderer.getDevice().createPipeline(&vertexLayoutDescription, vertexShader, pixelShader);
	vertexBuffer = renderer.getDevice().createBuffer(sge::BufferType::VERTEX, sge::BufferUsage::DYNAMIC, sizeof(vertexData));
	uniformBuffer = renderer.getDevice().createBuffer(sge::BufferType::UNIFORM, sge::BufferUsage::DYNAMIC, sizeof(UniformData));

	renderer.getDevice().bindViewport(&viewport);
	renderer.getDevice().bindPipeline(pipeline);
	renderer.getDevice().bindVertexBuffer(vertexBuffer);
	renderer.getDevice().copyData(vertexBuffer, sizeof(vertexData), vertexData);

	// TODO plan a simple (and smart) way to generate these commands.
	// Maybe we could generate it directly from renderdata?
	// These draw the triangle described in the vertex buffer to a given destination.

	// RenderData has all the data needed for rendering (d'oh). Vertex data, index data, matrices etc.
	// But we also have to somehow pass required shaders to renderer. Should this be done via material struct?
	// Shaders should be delivered so that you can sort the queue by them. This would optimize the drawing a bit
	// because all the data using the same shaders would be drawn in a one pass (?). This applies only to opaque
	// data, since transparent data should be sorted by their depth.

	createSprite({ 256.0f, 256.0f, 0.0f }, { 256.0f, 256.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f });
	createSprite({ 512.0f, 256.0f, 0.0f }, { 128.0f, 256.0f, 1.0f }, { 1.0f, 0.0f, 0.3f, 0.6f });
	
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

		renderer.getDevice().bindVertexUniformBuffer(uniformBuffer, 0);

		for (auto sprite : sprites)
		{
			renderer.pushCommand(sprite->getKey(), std::bind(&sge::SpriteComponent::render, sprite, std::placeholders::_1));
		}

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

	renderer.getDevice().deleteBuffer(vertexBuffer);
	renderer.getDevice().deleteBuffer(uniformBuffer);
	renderer.getDevice().deleteShader(vertexShader);
	renderer.getDevice().deleteShader(pixelShader);
	renderer.getDevice().deletePipeline(pipeline);

	renderer.deinit();

	SDL_Quit();

	return 0;
}