#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "SDL2/SDL.h"

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
	sge::GraphicsDevice device(window);

	device.init();

	std::vector<char> pShaderData;
	std::vector<char> vShaderData;

#ifdef DIRECTX11
	loadBinaryShader("../Assets/Shaders/SimpleVertexShader.cso", vShaderData);
	loadBinaryShader("../Assets/Shaders/SimplePixelShader.cso", pShaderData);
#elif OPENGL4
	loadTextShader("../Assets/Shaders/SimpleVertexShader.glsl", vShaderData);
	loadTextShader("../Assets/Shaders/SimplePixelShader.glsl", pShaderData);
#endif

	float vertexData[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	};

	sge::VertexLayoutDescription vertexLayoutDescription = { 2,
	{
		{ 0, 3, sge::VertexSemantic::POSITION },
		{ 0, 4, sge::VertexSemantic::COLOR }
	}};

	sge::Shader* vertexShader = device.createShader(sge::ShaderType::VERTEX, vShaderData.data(), vShaderData.size());
	sge::Shader* pixelShader = device.createShader(sge::ShaderType::PIXEL, pShaderData.data(), pShaderData.size());

	sge::Pipeline* pipeline = device.createPipeline(&vertexLayoutDescription, vertexShader, pixelShader);
	sge::Viewport viewport = { 0, 0, 1280, 720 };

	sge::Buffer* vertexBuffer = device.createBuffer(sge::BufferType::VERTEX, sge::BufferUsage::DYNAMIC, sizeof(vertexData));

	device.bindViewport(&viewport);
	device.bindPipeline(pipeline);

	device.bindVertexBuffer(vertexBuffer);
	device.copyData(vertexBuffer, sizeof(vertexData), vertexData);

	sge::RenderData renderData;
	sge::RenderCommand renderCommand = createCommand(0, 0, 0, 0, 0, 0);
	sge::RenderQueue renderQueue(1000);

	renderData.buffers.emplace_back(vertexBuffer);

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

		device.clear(0.5f, 0.0f, 0.5f, 1.0f);

		renderQueue.begin();

		renderQueue.push(renderCommand, &renderData);

		renderQueue.sort();

		device.draw(3);

		device.swap();
	}

	// Deinit

	device.debindPipeline(pipeline);

	device.deleteBuffer(vertexBuffer);

	device.deleteShader(vertexShader);
	device.deleteShader(pixelShader);

	device.deletePipeline(pipeline);
	device.deinit();

	SDL_Quit();

	return 0;
}