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

// TODO global is no no.
sge::Entity entity;
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

sge::SpriteComponent createSprite(const sge::math::vec3& position, const sge::math::vec3& scale, const sge::math::vec4& color, float rotation = 0)
{
	sge::SpriteComponent sprite(&entity);
	sprite.setPipeline(pipeline);
	sprite.setTexture(nullptr);
	sprite.setUniformBuffer(uniformBuffer);
	sprite.setVertexBuffer(vertexBuffer);
	sprite.setVP(VP);
	sprite.setColor(color);
	sprite.setPosition(position);
	sprite.setScale(scale);
	sprite.setRotation(rotation);

	return sprite;
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

	sge::SpriteComponent sprite1 = createSprite({ 256.0f, 256.0f, 0.0f }, { 256.0f, 256.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f });
	sge::SpriteComponent sprite2 = createSprite({ 512.0f, 256.0f, 0.0f }, { 128.0f, 256.0f, 1.0f }, { 1.0f, 0.0f, 0.3f, 0.6f }, sge::math::radians(90.0f));
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
		sprite2.setRotation(alpha);

		// Rendering
		renderer.begin();

		renderer.getDevice().bindVertexUniformBuffer(uniformBuffer, 0);

		renderer.pushCommand(sprite1.getKey(), std::bind(&sge::SpriteComponent::render, &sprite1, std::placeholders::_1));
		renderer.pushCommand(sprite2.getKey(), std::bind(&sge::SpriteComponent::render, &sprite2, std::placeholders::_1));

		renderer.end();
	}

	// Deinit
	renderer.getDevice().deleteBuffer(vertexBuffer);
	renderer.getDevice().deleteBuffer(uniformBuffer);
	renderer.getDevice().deleteShader(vertexShader);
	renderer.getDevice().deleteShader(pixelShader);
	renderer.getDevice().deletePipeline(pipeline);

	renderer.deinit();

	SDL_Quit();

	return 0;
}