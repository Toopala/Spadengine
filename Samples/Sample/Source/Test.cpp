#include "Core/Math.h"
#include "Renderer/Enumerations.h"
#include "Renderer/Window.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/Pipeline.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexLayout.h"
#include "Renderer/Viewport.h"
#include "SDL/SDL.h"
#include "Game/EntityManager.h"
#include "Core/Memory/PagePoolAllocator.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "Model.h"

class MemoryTest
{
public:
	MemoryTest(int a, int b, int c, std::string d)
	{
		testia = a;
		testib = b;
		testic = c;
		testid = d;
		std::cout << "Constructed" << std::endl;
	}
	~MemoryTest()
	{
		std::cout << "Destroyed!" << std::endl;
	}
	int testia, testib, testic;
	std::string testid;
};

namespace
{
	float lastX, lastY;
	float yaw, pitch;
	glm::vec3 cameraFront;
	int mouseXpos, mouseYpos;
	bool firstMouse = true;
}

// Mouse look sample
void mouseLook(int mouseX, int mouseY)
{
	if (firstMouse)
	{
		lastX += mouseXpos;
		lastY += mouseYpos;
		firstMouse = false;
	}

	float xoffset = mouseX - lastX;
	float yoffset = lastY - mouseY;
	lastX = mouseX;
	lastY = mouseY;

	float sensitivity = 0.55f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	sge::math::vec3 front;
	front.x = sge::math::cos(sge::math::radians(pitch)) * sge::math::cos(sge::math::radians(yaw));
	front.y = sge::math::sin(sge::math::radians(pitch));
	front.z = sge::math::cos(sge::math::radians(pitch)) * sge::math::sin(sge::math::radians(yaw));
	cameraFront = sge::math::normalize(front);
}

void loadTextShader(const std::string& path, std::vector<char>& data)
{
	std::ifstream file;

	file.open(path, ios::in);

	if (file.is_open())
	{
		std::cout << "Text shader opened!" << std::endl;

		std::stringstream stream;
		std::string str;

		stream << file.rdbuf();

		str = stream.str();

		std::copy(str.begin(), str.end(), std::back_inserter(data));

		data.push_back('\0');
	}
}

void loadBinaryShader(const std::string& path, std::vector<char>& data)
{
	std::ifstream file;

	file.open(path, ios::in | ios::ate | ios::binary);

	if (file.is_open())
	{
		std::cout << "Binary shader opened!" << std::endl;
		data.resize(static_cast<size_t>(file.tellg()));

		file.seekg(0, ios::beg);
		file.read(data.data(), data.size());
		file.close();
	}
}

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	sge::Window window("Spade Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720);
	sge::GraphicsDevice device(window);

	device.init();

	int w, h, n;

	unsigned char* data = stbi_load("rockwall_diffuse_map.png", &w, &h, &n, STBI_rgb_alpha);

	std::cout << "Opened image rockwall_diffuse_map.png: " << w << "x" << h << " and something like " << n << std::endl;

	unsigned char* data2 = stbi_load("rockwall_normal_map.png", &w, &h, &n, STBI_rgb_alpha);

	std::cout << "Opened image rockwall_normal_map.png: " << w << "x" << h << " and something like " << n << std::endl;

	std::vector<char> pShaderData;
	std::vector<char> vShaderData;

	// DX SHADERS

	loadBinaryShader("Assets/Shaders/VertexShader.cso", vShaderData);
	loadBinaryShader("Assets/Shaders/PixelShader.cso", pShaderData);

	// OPENGL SHADERS

	//loadTextShader("Assets/Shaders/VertexShader.glsl", vShaderData);
	//loadTextShader("Assets/Shaders/PixelShader.glsl", pShaderData);

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 4.5f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 P = glm::perspective(glm::radians(66.0f), 1280.0f / 720.0f, 0.1f, 1000.f);
	glm::mat4 V = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	struct UniformData
	{
		sge::math::mat4 PV;
		sge::math::mat4 M;
	} uniformData;

	uniformData.M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
	uniformData.PV = P * V;

	//Assimp test
	Model* model = new Model("cube.dae");

	std::vector<Vertex>* vertices = model->getVerticeArray();
	std::vector<unsigned int>* indices = model->getIndexArray();

	sge::VertexLayoutDescription vertexLayoutDescription = { 5,
	{
		{ 0, 3, sge::VertexSemantic::POSITION },
		{ 0, 3, sge::VertexSemantic::NORMAL },
		{ 0, 3, sge::VertexSemantic::TANGENT },
		{ 0, 3, sge::VertexSemantic::TANGENT },
		{ 0, 2, sge::VertexSemantic::TEXCOORD }
	} };

	sge::Shader* vertexShader = device.createShader(sge::ShaderType::VERTEX, vShaderData.data(), vShaderData.size());
	sge::Shader* pixelShader = device.createShader(sge::ShaderType::PIXEL, pShaderData.data(), pShaderData.size());

	sge::Texture* texture = device.createTexture(w, h, data);
	sge::Texture* texture2 = device.createTexture(w, h, data2);

	stbi_image_free(data);
	stbi_image_free(data2);

	sge::Pipeline* pipeline = device.createPipeline(&vertexLayoutDescription, vertexShader, pixelShader);
	sge::Viewport viewport = { 0, 0, 1280, 720 };

	sge::Buffer* vertexBuffer = device.createBuffer(sge::BufferType::VERTEX, sge::BufferUsage::DYNAMIC, sizeof(Vertex) * vertices->size());
	sge::Buffer* uniformBuffer = device.createBuffer(sge::BufferType::UNIFORM, sge::BufferUsage::DYNAMIC, sizeof(uniformData));

	device.bindViewport(&viewport);
	device.bindPipeline(pipeline);

	device.bindVertexBuffer(vertexBuffer);
	device.bindVertexUniformBuffer(uniformBuffer, 0);
	device.bindTexture(texture, 0);
	device.bindTexture(texture2, 1);

	device.copyData(vertexBuffer, sizeof(Vertex) * vertices->size(), vertices->data());
	device.copyData(uniformBuffer, sizeof(uniformData), &uniformData);

	SDL_Event event;

	bool running = true;

	float temp = 0;

	// Memory allocation test

	MemoryTest *mt1 = sge::allocator.create<MemoryTest>(10, 10, 10, "10");
	sge::allocator.destroy<MemoryTest>(mt1);

	MemoryTest *mt5 = sge::allocator.create<MemoryTest>(1, 2, 3, "4");
	sge::allocator.destroy<MemoryTest>(mt5);

	float alpha = 0.0f;

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

		// Mouse Look sample
#ifdef _WIN32
		SDL_GetGlobalMouseState(&mouseXpos, &mouseYpos);
		std::cout << mouseXpos << " - " << mouseYpos << std::endl;

		mouseLook(mouseXpos, mouseYpos);
		//SDL_WarpMouseInWindow(window.getSDLWindow(),window.getWidth()/2, window.getHeight()/2);
#endif
		V = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		uniformData.PV = P*V;

		// If placing M (model matrix) into equation then no need for increasing alpha (for angle) or translate location. These functions now add to the previous result.  
		uniformData.M = sge::math::rotate(uniformData.M, glm::radians(0.1f), glm::vec3(1.0f, 1.0f, 1.0f));

		device.copyData(uniformBuffer, sizeof(uniformData), &uniformData);

		device.draw(vertices->size());

		device.swap();

		alpha += 0.01f;
	}

	device.debindPipeline(pipeline);

	device.deleteBuffer(vertexBuffer);
	device.deleteBuffer(uniformBuffer);

	device.deleteShader(vertexShader);
	device.deleteShader(pixelShader);
	device.deleteTexture(texture);
	device.deleteTexture(texture2);

	device.deletePipeline(pipeline);

	device.deinit();

	SDL_Quit();

	return 0;
}