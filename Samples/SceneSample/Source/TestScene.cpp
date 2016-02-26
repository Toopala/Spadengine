#include "TestScene.h"

#include "stb_image.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "Model.h"

#include "Renderer/Enumerations.h"
#include "Renderer/Window.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/Pipeline.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexLayout.h"

//--------------------------------
// Copied functions


// Mouse look sample
void TestScene::mouseLook(int mouseX, int mouseY)
{
	if (firstMouse)
	{
		lastX += mouseXpos;
		lastY += mouseYpos;
		firstMouse = false;
	}

	mousseX += mouseX;
	mousseY += mouseY;

	float xoffset = mousseX - lastX;
	float yoffset = lastY - mousseY;
	lastX = mousseX;
	lastY = mousseY;

	float sensitivity = 0.15f;
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

void TestScene::loadTextShader(const std::string& path, std::vector<char>& data)
{
	std::ifstream file;

	file.open(path, std::ios::in);

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

void TestScene::loadBinaryShader(const std::string& path, std::vector<char>& data)
{
	std::ifstream file;

	file.open(path, std::ios::in | std::ios::ate | std::ios::binary);

	if (file.is_open())
	{
		std::cout << "Binary shader opened!" << std::endl;
		data.resize(static_cast<size_t>(file.tellg()));

		file.seekg(0, std::ios::beg);
		file.read(data.data(), data.size());
		file.close();
	}
}
// Copied functions ends
//--------------------------------

TestScene::TestScene(sge::Spade* engine) : engine(engine)
{
	std::cout << "test scene constructor says hello" << std::endl;
	mouseX = 0;
	mouseY = 0;

	engine->mouseInput->enableRelativeMousePosition();
	// Shaders and stuff here

	int w, h, n;

	unsigned char* data = stbi_load("../Assets/rockwall_diffuse_map.png", &w, &h, &n, STBI_rgb_alpha);

	std::cout << "Opened image rockwall_diffuse_map.png: " << w << "x" << h << " and something like " << n << std::endl;

	unsigned char* data2 = stbi_load("../Assets/rockwall_normal_map.png", &w, &h, &n, STBI_rgb_alpha);

	std::cout << "Opened image rockwall_normal_map.png: " << w << "x" << h << " and something like " << n << std::endl;

	std::vector<char> pShaderData;
	std::vector<char> vShaderData;

#ifdef DIRECTX11
	loadBinaryShader("../Assets/Shaders/VertexShader.cso", vShaderData);
	loadBinaryShader("../Assets/Shaders/PixelShader.cso", pShaderData);
#elif OPENGL4
	loadTextShader("../Assets/Shaders/VertexShader.glsl", vShaderData);
	loadTextShader("../Assets/Shaders/PixelShader.glsl", pShaderData);
#endif

	cameraPos = glm::vec3(0.0f, 0.0f, 4.5f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	P = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 1000.f);
	V = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	uniformData.M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
	uniformData.PV = P * V;

	//Assimp test
	Model* model = new Model("../Assets/cube.dae");

	vertices = model->getVerticeArray();
	indices = model->getIndexArray();

	sge::VertexLayoutDescription vertexLayoutDescription = { 5,
	{
		{ 0, 3, sge::VertexSemantic::POSITION },
		{ 0, 3, sge::VertexSemantic::NORMAL },
		{ 0, 3, sge::VertexSemantic::TANGENT },
		{ 0, 3, sge::VertexSemantic::TANGENT },
		{ 0, 2, sge::VertexSemantic::TEXCOORD }
	} };

	vertexShader = engine->getDevice().createShader(sge::ShaderType::VERTEX, vShaderData.data(), vShaderData.size());
	pixelShader = engine->getDevice().createShader(sge::ShaderType::PIXEL, pShaderData.data(), pShaderData.size());

	texture = engine->getDevice().createTexture(w, h, data);
	texture2 = engine->getDevice().createTexture(w, h, data2);

	stbi_image_free(data);
	stbi_image_free(data2);

	pipeline = engine->getDevice().createPipeline(&vertexLayoutDescription, vertexShader, pixelShader);
	viewport = { 0, 0, 1280, 720 };

	vertexBuffer = engine->getDevice().createBuffer(sge::BufferType::VERTEX, sge::BufferUsage::DYNAMIC, sizeof(Vertex) * vertices->size());
	uniformBuffer = engine->getDevice().createBuffer(sge::BufferType::UNIFORM, sge::BufferUsage::DYNAMIC, sizeof(uniformData));

	engine->getDevice().bindViewport(&viewport);
	engine->getDevice().bindPipeline(pipeline);

	engine->getDevice().bindVertexBuffer(vertexBuffer);
	engine->getDevice().bindVertexUniformBuffer(uniformBuffer, 0);
	engine->getDevice().bindTexture(texture, 0);
	engine->getDevice().bindTexture(texture2, 1);

	engine->getDevice().copyData(vertexBuffer, sizeof(Vertex) * vertices->size(), vertices->data());
	engine->getDevice().copyData(uniformBuffer, sizeof(uniformData), &uniformData);

	SDL_Event event;

	bool running = true;

	alpha = 0.0f;

	useMouse = true;
	if (useMouse) SDL_SetRelativeMouseMode(SDL_TRUE);
}

TestScene::~TestScene()
{
	std::cout << "test scene terminator says hello" << std::endl;
	engine->getDevice().debindPipeline(pipeline);

	engine->getDevice().deleteBuffer(vertexBuffer);
	engine->getDevice().deleteBuffer(uniformBuffer);

	engine->getDevice().deleteShader(vertexShader);
	engine->getDevice().deleteShader(pixelShader);
	engine->getDevice().deleteTexture(texture);
	engine->getDevice().deleteTexture(texture2);

	engine->getDevice().deletePipeline(pipeline);
}

void TestScene::update(float step)
{		
	if (useMouse)
	{
#ifdef _WIN32
		engine->mouseInput->getRelativeMouseState(&mouseXpos, &mouseYpos);

		mouseLook(mouseXpos, mouseYpos);
#endif
		V = sge::math::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		uniformData.PV = P*V;
	}

	// Eemeli nyt oikeasti tämä rotate tehdään näin! Muuten tulee salmiakkia.
	uniformData.M = sge::math::rotate(sge::math::mat4(), alpha, glm::vec3(1.0f, 1.0f, 1.0f));

	if (engine->mouseInput->buttonIsPressed(sge::MOUSE_BUTTON_LEFT))
	{
		// Proper way to shutdown the program?
		engine->stop();
	}

	alpha += 0.005;
}

void TestScene::draw()
{
	engine->getDevice().clear(0.5f, 0.0f, 0.5f, 1.0f);

	engine->getDevice().copyData(uniformBuffer, sizeof(uniformData), &uniformData);

	engine->getDevice().draw(vertices->size());

	engine->getDevice().swap();
}

void TestScene::interpolate(float alpha)
{

}
