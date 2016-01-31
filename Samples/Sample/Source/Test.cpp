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

	device.init();

	const char* VERTEX_SOURCE =
		"#version 420\n"

		"in vec3 inPosition;\n"
		"in vec4 inColor;\n"
		"out vec4 fColor;\n"
		"layout (std140) uniform MVP\n"
		"{\n"
		"mat4 model;\n"
		"mat4 view;\n"
		"mat4 projection;\n"
		"};\n"

	"void main()\n"
	"{\n"
	"	gl_Position = projection * view * model * vec4(inPosition, 1.0);\n"
		"   fColor = inColor;\n"
	"}\n";

	const char* PIXEL_SOURCE =
	"#version 420\n"

		"in vec4 fColor;\n"
	"out vec4 outColour;\n"

	"void main()\n"
	"{\n"
		"	outColour = fColor;\n"
	"}\n";

	float width = 256.0f;
	float height = 256.0f;

	float vertexData[] = 
	{ 
		-width, height, 0.0f, 1.0f,		0.0f, 0.0f, 1.0f,
		width, height, 0.0f, 0.0f,		1.0f, 0.0f, 1.0f,
		width, -height, 0.0f, 0.0f,		0.0f, 1.0f, 1.0f,
		-width, -height, 0.0f, 1.0f,	1.0f, 1.0f, 1.0f,
	};

	sge::math::mat4 uniformData[] =
	{
		sge::math::translate(sge::math::mat4(1.0f), sge::math::vec3(window.getWidth() / 2.0f - width, window.getHeight() / 2.0f, 0.0f)),
		sge::math::mat4(1.0f),
		sge::math::mat4(sge::math::ortho(0.0f, 1280.0f, 720.0f, 0.0f))
	};

	//Assimp test
	Model* model = new Model("plane.dae");

	std::vector<Vertex>* vertices = model->getVerticeArray();
	std::vector<unsigned int>* indices = model->getIndexArray();

	unsigned int indexData[] =
	{
		0, 1, 2, 0, 2, 3
	};

	sge::VertexLayoutDescription vertexLayoutDescription = { 2, { 3, 4 } };

	sge::Shader* vertexShader = device.createShader(sge::ShaderType::VERTEX, VERTEX_SOURCE);
	sge::Shader* pixelShader = device.createShader(sge::ShaderType::PIXEL, PIXEL_SOURCE);

	sge::Pipeline* pipeline = device.createPipeline(&vertexLayoutDescription, vertexShader, pixelShader);
	sge::Viewport viewport = { 0, 0, 1280, 720 };

	sge::Buffer* vertexBuffer = device.createBuffer(sge::BufferType::VERTEX, sge::BufferUsage::STATIC);
	sge::Buffer* indexBuffer = device.createBuffer(sge::BufferType::INDEX, sge::BufferUsage::STATIC);
	sge::Buffer* uniformBuffer = device.createBuffer(sge::BufferType::UNIFORM, sge::BufferUsage::STATIC);

	device.bindViewport(&viewport);
	device.bindPipeline(pipeline);

	device.bindVertexBuffer(vertexBuffer);
	device.bindIndexBuffer(indexBuffer);
	device.bindVertexUniformBuffer(uniformBuffer);

	device.copyData(vertexBuffer, sizeof(vertexData), vertexData);
	device.copyData(indexBuffer, sizeof(indexData), indexData);
	device.copyData(uniformBuffer, sizeof(uniformData), uniformData);

	SDL_Event event;

	bool running = true;

	float temp = 0;

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

		uniformData[0] = sge::math::translate(uniformData[0], glm::vec3(sge::math::sin(temp += 0.025f) * 6, 0.0f, 0.0f));

		device.copySubData(uniformBuffer, 0, sizeof(sge::math::mat4), uniformData);
		
		device.drawIndexed(6);

		window.swap();
	}

	device.debindPipeline(pipeline);

	device.deleteBuffer(indexBuffer);
	device.deleteBuffer(vertexBuffer);
	device.deleteBuffer(uniformBuffer);

	device.deleteShader(vertexShader);
	device.deleteShader(pixelShader);

	device.deletePipeline(pipeline);
	
	device.deinit();

	SDL_Quit();

	return 0;
}