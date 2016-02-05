#include "Core/Math.h"
#include "Renderer/Window.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/Pipeline.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexLayout.h"
#include "Renderer/Viewport.h"
#include "SDL/SDL.h"


#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

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

	int w, h, n;

	unsigned char* data = stbi_load("rockwall_diffuse_map.png", &w, &h, &n, STBI_rgb_alpha);

	std::cout << "Opened image spade.png: " << w << "x" << h << " and something like " << n << std::endl;




	const char* VERTEX_SOURCE =
		"#version 440\n"

		"in vec3 inPosition;\n"
		"in vec3 inNormal;\n"
		"in vec2 inTexcoords;\n"

		"out vec2 texcoords;\n"

		"layout (std140, binding = 0) uniform MVPUniform\n"
		"{\n"
		"mat4 MVP;\n"
		"};\n"

		"void main()\n"
		"{\n"
		"vec3 pos = inPosition;\n"
		"pos = pos * gl_InstanceID;\n"
		"gl_Position = vec4(pos, 1.0);\n"
		"texcoords = inTexcoords;\n"
		"}\n";

	const char* PIXEL_SOURCE =
		"#version 440\n"

		"in vec2 texcoords;\n"
		"out vec4 outColour;\n"

		"layout(binding = 0) uniform sampler2D tex;\n"

		"void main()\n"
		"{\n"
		"	outColour = texture2D(tex, texcoords);\n"
		"}\n";

	float width = 0.1f;
	float height = 0.1f;


	float vertexData[] =
	{
		width, 2 * height, 0.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
		2 * width, 2 * height, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		2 * width, height, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		width, height, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	};

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 4.5f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 P = glm::perspective(glm::radians(66.0f), 1280.0f / 720.0f, 0.1f, 1000.f);
	glm::mat4 V = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	glm::mat4 VP = P*V;

	glm::mat4 M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));

	glm::mat4 MVP = VP*M;

	sge::math::mat4 uniformData[] =
	{
		MVP
	};

	//Assimp test
	Model* model = new Model("plane.dae");

	std::vector<Vertex>* vertices = model->getVerticeArray();
	std::vector<unsigned int>* indices = model->getIndexArray();

	unsigned int indexData[] =
	{
		0, 1, 2, 0, 2, 3
	};

	sge::VertexLayoutDescription vertexLayoutDescription = { 3, { 3, 3, 2 } };

	sge::Shader* vertexShader = device.createShader(sge::ShaderType::VERTEX, VERTEX_SOURCE);
	sge::Shader* pixelShader = device.createShader(sge::ShaderType::PIXEL, PIXEL_SOURCE);
	sge::Texture* texture = device.createTexture(w, h, data);

	stbi_image_free(data);

	sge::Pipeline* pipeline = device.createPipeline(&vertexLayoutDescription, vertexShader, pixelShader);
	sge::Viewport viewport = { 0, 0, 1280, 720 };

	sge::Buffer* vertexBuffer = device.createBuffer(sge::BufferType::VERTEX, sge::BufferUsage::STATIC);
	sge::Buffer* indexBuffer = device.createBuffer(sge::BufferType::INDEX, sge::BufferUsage::STATIC);
	sge::Buffer* uniformBuffer = device.createBuffer(sge::BufferType::UNIFORM, sge::BufferUsage::STATIC);

	device.bindViewport(&viewport);
	device.bindPipeline(pipeline);

	device.bindVertexBuffer(vertexBuffer);
	device.bindIndexBuffer(indexBuffer);
	device.bindVertexUniformBuffer(uniformBuffer, 0);
	device.bindTexture(texture, 0);

	//device.copyData(vertexBuffer, vertices->size() * sizeof(Vertex), vertices->data());
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

		// If placing M (model matrix) into equation then no need for increasing alpha (for angle) or translate location. These functions now add to the previous result.  
		M = sge::math::rotate(M, glm::radians(5.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		
		// Order must not be changed.
		MVP = VP*M;
		uniformData[0] = MVP;

		device.copySubData(uniformBuffer, 0, sizeof(sge::math::mat4), uniformData);

		device.drawInstancedIndexed(6, 5);

		window.swap();		
	}

	device.debindPipeline(pipeline);

	device.deleteBuffer(indexBuffer);
	device.deleteBuffer(vertexBuffer);
	device.deleteBuffer(uniformBuffer);

	device.deleteShader(vertexShader);
	device.deleteShader(pixelShader);
	device.deleteTexture(texture);

	device.deletePipeline(pipeline);

	device.deinit();

	SDL_Quit();

	return 0;
}