#include "Core/Math.h"
#include "Renderer/Window.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/Pipeline.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexLayout.h"
#include "Renderer/Viewport.h"
#include "SDL/SDL.h"

#include <iostream>

#include "Model.h"

int main(int argc, char** argv)
{

	SDL_Init(SDL_INIT_VIDEO);

	sge::math::vec2 vec;
	std::cout << vec.x << ", " << vec.y << ", " << sge::math::haeSata() << std::endl;

	sge::Window window("Spade Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720);
	sge::GraphicsDevice device(window);

	const char* VERTEX_SOURCE =
	"#version 420\n"

	"in vec3 inPosition;\n"

	"void main()\n"
	"{\n"
	"	gl_Position = vec4(inPosition, 1.0);\n"
	"}\n";

	const char* PIXEL_SOURCE =
	"#version 420\n"

	"out vec4 outColour;\n"

	"void main()\n"
	"{\n"
	"	outColour = vec4(1.0, 1.0, 1.0, 1.0);\n"
	"}\n";

	float vertexData[] = 
	{ 
		-0.8f, 0.8f, 0.0f,
		0.8f, 0.8f, 0.0f,
		0.8f, -0.8f, 0.0f,
		-0.8f, -0.8f, 0.0f
	};

	//Assimp test
	Model* model = &Model("cube.dae");

	model->getMeshes();

	short indexData[] =
	{
		0, 1, 2, 0, 2, 3
	};

	sge::Shader* vertexShader = device.createShader(sge::ShaderType::VERTEX, VERTEX_SOURCE);
	sge::Shader* pixelShader = device.createShader(sge::ShaderType::PIXEL, PIXEL_SOURCE);
	sge::Buffer* vertexBuffer = device.createBuffer(sge::BufferType::VERTEX, sge::BufferUsage::STATIC);
	sge::Buffer* indexBuffer = device.createBuffer(sge::BufferType::INDEX, sge::BufferUsage::STATIC);
	sge::Pipeline* pipeline = device.createPipeline(vertexShader, pixelShader);
	sge::VertexLayoutDescription vertexLayoutDescription;

	device.bindPipeline(pipeline);
	device.bindVertexBuffer(vertexBuffer);
	device.bindIndexBuffer(indexBuffer);

	pipeline->vertexLayout = device.createVertexLayout(&vertexLayoutDescription, vertexShader);
	
	sge::Viewport viewport = { 0, 0, 1280, 720 };

	device.bindViewport(&viewport);

	device.copyData(vertexBuffer, vertexData, sizeof(vertexData));
	device.copyData(indexBuffer, indexData, sizeof(indexData));

	SDL_Event event;

	bool running = true;

	while (running)
	{
		device.clear(0.5f, 0.0f, 0.5f, 1.0f);

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			}
		}
		
		device.drawIndexed(6);

		window.swap();
	}

	device.debindPipeline(pipeline);

	device.deleteBuffer(indexBuffer);
	device.deleteBuffer(vertexBuffer);
	device.deleteShader(vertexShader);
	device.deleteShader(pixelShader);
	device.deletePipeline(pipeline);

	SDL_Quit();

	return 0;
}