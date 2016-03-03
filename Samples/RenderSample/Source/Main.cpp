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

sge::RenderCommand createCommand(uint64 data, uint64 fullscreenLayer, uint64 translucent, uint64 viewport, uint64 viewportLayer, uint64 command)
{
	sge::RenderCommand com;
	com.data = data;
	com.fullscreenLayer = fullscreenLayer;
	com.translucent = translucent;
	com.viewport = viewport;
	com.viewportLayer = viewportLayer;
	com.command = command;

	return com;
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

	// TODO add uniform buffers so you can use one vertex data for multiple primitives.
	float width = 256.0f;
	float height = 256.0f;

	float vertexData[] = {
		0.0f, height, 0.0f,
		width, -height, 0.0f,
		-width, -height, 0.0f,
	};

	// TODO load layout description from external file (shader.reflect).

	sge::VertexLayoutDescription vertexLayoutDescription = { 1,
	{
		{ 0, 3, sge::VertexSemantic::POSITION }
	}};

	// TODO move shader, pipeline, buffer and texture creation to somewhere else.
	// PLAN CAREFULLY.

	// What if render material had pipeline? And using material would bind it.

	sge::Shader* vertexShader = renderer.getDevice().createShader(sge::ShaderType::VERTEX, vShaderData.data(), vShaderData.size());
	sge::Shader* pixelShader = renderer.getDevice().createShader(sge::ShaderType::PIXEL, pShaderData.data(), pShaderData.size());

	sge::Pipeline* pipeline = renderer.getDevice().createPipeline(&vertexLayoutDescription, vertexShader, pixelShader);
	sge::Viewport viewport = { 0, 0, 1280, 720 };

	sge::Buffer* vertexBuffer = renderer.getDevice().createBuffer(sge::BufferType::VERTEX, sge::BufferUsage::DYNAMIC, sizeof(vertexData));

	renderer.getDevice().bindViewport(&viewport);
	renderer.getDevice().bindPipeline(pipeline);

	renderer.getDevice().bindVertexBuffer(vertexBuffer);
	renderer.getDevice().copyData(vertexBuffer, sizeof(vertexData), vertexData);


	// TODO plan a simple (and smart) way to generate these commands.
	// Maybe we could generate it directly from renderdata?
	// These draw the triangle described in the vertex buffer to a given destination.

	sge::RenderData renderData;
	sge::RenderData renderData2;
	sge::RenderData renderData3;
	sge::RenderData renderData4;
	sge::RenderData renderData5;

	renderData.buffers.emplace_back(vertexBuffer);
	renderData.count = 3;
	renderData.pos = { 1280.0f/2, 720.0f/2, 0.0f };
	renderData.color = { 1.0f, 0.0f, 0.0f, 1.0f };

	renderData2.buffers.emplace_back(vertexBuffer);
	renderData2.count = 3;
	renderData2.pos = { 256.0f, 256.0f, 0.0f };
	renderData2.color = { 1.0f, 1.0f, 1.0f, 0.5f };

	renderData3.buffers.emplace_back(vertexBuffer);
	renderData3.count = 3;
	renderData3.pos = { 1280.0f / 2, 0.0f, 0.0f };
	renderData3.color = { 1.0f, 1.0f, 0.0f, 0.4f };

	renderData4.buffers.emplace_back(vertexBuffer);
	renderData4.count = 3;
	renderData4.pos = { 512.0f, 256.0f, 0.0f };
	renderData4.color = { 1.0f, 0.0f, 1.0f, 1.0f };

	renderData5.buffers.emplace_back(vertexBuffer);
	renderData5.count = 3;
	renderData5.pos = { 1280.0f/2, 256.0f, 0.0f };
	renderData5.color = { 1.0f, 0.0f, 0.7f, 0.8f };

	// Loop
	SDL_Event event;
	bool running = true;

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

		// Rendering
		renderer.begin();

		renderer.pushCommand(createCommand(0, 0, 0, 0, 0, 0), &renderData);
		renderer.pushCommand(createCommand(4, 0, 0, 0, 0, 0), &renderData2);
		renderer.pushCommand(createCommand(2, 0, 0, 0, 0, 0), &renderData3);
		renderer.pushCommand(createCommand(3, 0, 0, 0, 0, 0), &renderData4);
		renderer.pushCommand(createCommand(1, 0, 0, 0, 0, 0), &renderData5);

		renderer.end();
	}

	// Deinit
	renderer.getDevice().debindPipeline(pipeline);

	renderer.getDevice().deleteBuffer(vertexBuffer);

	renderer.getDevice().deleteShader(vertexShader);
	renderer.getDevice().deleteShader(pixelShader);

	renderer.getDevice().deletePipeline(pipeline);

	renderer.deinit();

	SDL_Quit();

	return 0;
}