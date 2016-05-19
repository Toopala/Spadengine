#include "Scene.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "Renderer/Enumerations.h"
#include "Renderer/Window.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/Pipeline.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexLayout.h"

#include "Core/Random.h"
#include "Game/TransformComponent.h"

#include"Audio/Audio.h"

Scene::Scene(sge::Spade *engine) : engine(engine), renderer(engine->getRenderer())
{

	// Entity manager:
	entityManager = new sge::EntityManager();
	// ----------------------

	// Shaders:
	sge::VertexLayoutDescription vertexLayoutDescription = { 5,
	{
		{ 0, 3, sge::VertexSemantic::POSITION },
		{ 0, 3, sge::VertexSemantic::NORMAL },
		{ 0, 3, sge::VertexSemantic::TANGENT },
		{ 0, 3, sge::VertexSemantic::TANGENT },
		{ 0, 2, sge::VertexSemantic::TEXCOORD }
	} };

	std::vector<char> pShaderDataNormals;
	std::vector<char> vShaderDataNormals;

#ifdef DIRECTX11
	loadBinaryShader("../../Shaders/Compiled/VertexShaderLights.cso", vShaderDataNormals);
	loadBinaryShader("../../Shaders/Compiled/PixelShaderLights.cso", pShaderDataNormals);
#elif OPENGL4
	loadTextShader("../Assets/Shaders/VertexShaderLights.glsl", vShaderDataNormals);
	loadTextShader("../Assets/Shaders/PixelShaderLights.glsl", pShaderDataNormals);
#endif

	vertexShader = engine->getRenderer()->getDevice()->createShader(sge::ShaderType::VERTEX, vShaderDataNormals.data(), vShaderDataNormals.size());
	pixelShader = engine->getRenderer()->getDevice()->createShader(sge::ShaderType::PIXEL, pShaderDataNormals.data(), pShaderDataNormals.size());

	pipeline = engine->getRenderer()->getDevice()->createPipeline(&vertexLayoutDescription, vertexShader, pixelShader);
	engine->getRenderer()->getDevice()->bindPipeline(pipeline);
	// ----------------------

	// Lights:
	sge::PointLight pointLight;
	pointLight.position = sge::math::vec4(0.0);
	pointLight.constant = 1.0;
	pointLight.mylinear = 0.09;
	pointLight.quadratic = 0.0032;
	pointLight.pad = 0.0;
	pointLight.ambient = sge::math::vec4(1.0, 0.1, 0.1, 1);
	pointLight.diffuse = sge::math::vec4(5.0, 1.0, 1.0, 1.0);
	pointLight.specular = sge::math::vec4(1.0, 1.0, 1.0, 1.0);

	pointLightEntity = entityManager->createEntity();
	pointLightTransform = new sge::TransformComponent(pointLightEntity);
	pointLightTransform->setPosition({ 3.0, 4.0, 0.0 });
	pointLightEntity->setComponent(pointLightTransform);
	pointLightComponent = new sge::PointLightComponent(pointLightEntity);
	pointLightComponent->setLightData(pointLight);
	pointLightEntity->setComponent(pointLightComponent);

	pointLights.push_back(pointLightEntity);

	sge::PointLight pointLight2;
	pointLight2.position = sge::math::vec4(0.0);
	pointLight2.constant = 1.0;
	pointLight2.mylinear = 0.09;
	pointLight2.quadratic = 0.0032;
	pointLight2.pad = 0.0;
	pointLight2.ambient = sge::math::vec4(0.1, 0.1, 1.0, 1);
	pointLight2.diffuse = sge::math::vec4(1.0, 1.0, 5.0, 1.0);
	pointLight2.specular = sge::math::vec4(1.0, 1.0, 1.0, 1.0);

	pointLightEntity2 = entityManager->createEntity();
	pointLightTransform2 = new sge::TransformComponent(pointLightEntity2);
	pointLightTransform2->setPosition({ 3.0, 4.0, 0.0 });
	pointLightEntity2->setComponent(pointLightTransform2);
	pointLightComponent2 = new sge::PointLightComponent(pointLightEntity2);
	pointLightComponent2->setLightData(pointLight2);
	pointLightEntity2->setComponent(pointLightComponent2);

	pointLights.push_back(pointLightEntity2);

	sge::DirLight dirLight;
	dirLight.ambient = sge::math::vec4(0.1, 0.5, 0.1, 1);
	dirLight.diffuse = sge::math::vec4(0.1, 0.5, 0.1, 1.0);
	dirLight.specular = sge::math::vec4(1.0, 1.0, 1.0, 1.0);
	dirLight.direction = sge::math::vec4(0.0,-1.0,0.0,0.0);

	dirLightEntity = entityManager->createEntity();
	dirLightTransform = new sge::TransformComponent(dirLightEntity);
	dirLightTransform->setPosition({ 0.0, -10.0, 0.0 });
	dirLightEntity->setComponent(dirLightTransform);
	dirLightComponent = new sge::DirLightComponent(dirLightEntity);
	dirLightComponent->setLightData(dirLight);
	dirLightEntity->setComponent(dirLightComponent);

	dirLights.push_back(dirLightEntity);
	// ----------------------

	// Camera:
	sge::Entity* camera = entityManager->createEntity();

	cameraTransform = new sge::TransformComponent(camera);
	cameraPos = glm::vec3(0.0, 0.0, -5.0);
	cameraFront = glm::vec3(0.0, 0.0, -5.0);
	cameraUp = glm::vec3(0.0, 1.0, 0.0);
	cameraTransform->setPosition(cameraPos);
	cameraTransform->setFront(cameraFront);
	cameraTransform->setUp(cameraUp);
	camera->setComponent(cameraTransform);

	cameraComponent = new sge::CameraComponent(camera);
	cameraComponent->setPerspective(45.0, 16.0/9.0, 0.1, 1000.0);
	cameraComponent->setViewport(0, 0, 1280, 720);
	camera->setComponent(cameraComponent);

	cameras.push_back(camera);
	// ----------------------

	// Cube model
	cubeHandle = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/cubeSpecularNormal.dae");
	cubeHandle.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());
	cubeEntity = entityManager->createEntity();

	cubeTransform = new sge::TransformComponent(cubeEntity);
	cubeTransform->setPosition({ 0.0, -10.0, 0.0 });
	cubeTransform->setRotationVector({ 1.0, 0.0, 0.0 });
	cubeTransform->setScale({10.0, 10.0, 10.0});
	cubeTransform->setAngle(-90);
	cubeEntity->setComponent(cubeTransform);

	cubeComponent = new sge::ModelComponent(cubeEntity);
	cubeComponent->setShininess(10);
	cubeComponent->setModelResource(&cubeHandle);
	cubeComponent->setRenderer(engine->getRenderer());
	cubeComponent->setPipeline(pipeline);
	cubeEntity->setComponent(cubeComponent);

	cubeHandle.getResource<sge::ModelResource>()->createBuffers();
	gameObjects.push_back(cubeEntity);
	// ----------------------

	// Room model
	roomHandle = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/RoomBoxBig.dae");
	roomHandle.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());
	roomEntity = entityManager->createEntity();

	roomTransform = new sge::TransformComponent(roomEntity);
	roomTransform->setPosition({ 0.0, 0.0, 0.0 });
	roomTransform->setRotationVector({ 1.0, 0.0, 0.0 });
	roomTransform->setScale({ 0.6, 0.6, 0.6 });
	roomEntity->setComponent(roomTransform);

	roomComponent = new sge::ModelComponent(roomEntity);
	roomComponent->setShininess(250);
	roomComponent->setModelResource(&roomHandle);
	roomComponent->setRenderer(engine->getRenderer());
	roomComponent->setPipeline(pipeline);
	roomEntity->setComponent(roomComponent);

	roomHandle.getResource<sge::ModelResource>()->createBuffers();
	gameObjects.push_back(roomEntity);
	// ----------------------

	/*
	// Car model
	carHandle = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/carDiffuseSpecular.dae");
	carHandle.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());
	carEntity = entityManager->createEntity();

	carTransform = new sge::TransformComponent(carEntity);
	carTransform->setPosition({ 10.0, 10.0, 0.0 });
	carTransform->setRotationVector({ 0.0, 0.0, 1.0 });
	carTransform->setScale({ 10.0, 10.0, 10.0 });
	carEntity->setComponent(carTransform);

	carComponent = new sge::ModelComponent(carEntity);
	carComponent->setShininess(100);
	carComponent->setModelResource(&carHandle);
	carComponent->setRenderer(engine->getRenderer());
	carComponent->setPipeline(pipeline);
	carEntity->setComponent(carComponent);

	carHandle.getResource<sge::ModelResource>()->createBuffers();
	gameObjects.push_back(carEntity);
	// ----------------------
	*/
}

Scene::~Scene()
{
	sge::ResourceManager::getMgr().release(cubeHandle);
	engine->getRenderer()->getDevice()->debindPipeline(pipeline);
}

void Scene::update(float dt)
{
	// Mouse inputs:
#ifdef _WIN32
	if (useMouse)
	{
		engine->mouseInput->getRelativeMouseState(&mouse.x, &mouse.y);
		mouseLook();
		cameras[0]->getComponent<sge::TransformComponent>()->setFront(cameraFront);
	}
#endif

	// ----------------------

	// Keyboard inputs:
	if (useMouse)
	{
		if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_Q))
		{
			cameras[0]->getComponent<sge::TransformComponent>()->addPosition(cameraFront * movementSpeed);
		}
		if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_E))
		{
			cameras[0]->getComponent<sge::TransformComponent>()->addPosition(cameraFront * -movementSpeed);
		}
		if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_W))
		{
			cameras[0]->getComponent<sge::TransformComponent>()->addPosition(cameraUp * movementSpeed);
		}
		if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_S))
		{
			cameras[0]->getComponent<sge::TransformComponent>()->addPosition(cameraUp * -movementSpeed);
		}
		if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_A))
		{
			cameras[0]->getComponent<sge::TransformComponent>()->addPosition(-sge::math::cross(cameraFront, cameraUp) * movementSpeed);
		}
		if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_D))
		{
			cameras[0]->getComponent<sge::TransformComponent>()->addPosition(sge::math::cross(cameraFront, cameraUp) * movementSpeed);
		}
	}

	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_1) && !useMouse)
	{
		useMouse = true;
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_2) && useMouse)
	{
		useMouse = false;
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}

	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_ESCAPE))
	{
		engine->stop();
	}
	// ----------------------

	// Light update:
	alpha += 0.01f;
	float lightX = -25.0f*cos(alpha);
	float lightY = -25.0f*sin(alpha);
	float lightZ = -25.0f*sin(alpha);
	pointLights[0]->getComponent<sge::TransformComponent>()->setPosition(sge::math::vec3(lightX, lightY, lightZ));
	pointLights[1]->getComponent<sge::TransformComponent>()->setPosition(sge::math::vec3(lightX, lightY, lightZ) * glm::vec3(-1));
	for (auto pointLights : pointLights)
	{
		pointLights->getComponent<sge::PointLightComponent>()->update();
	}
	for (auto dirLights : dirLights)
	{
		dirLights->getComponent<sge::DirLightComponent>()->update();
	}
	// ----------------------

	// Camera update:
	for (auto camera : cameras)
	{
		camera->getComponent<sge::CameraComponent>()->update();
	}
	// ----------------------
}

void Scene::draw()
{
	renderer->addCameras(1, &cameras.front());
	renderer->setClearColor(0.0, 0.3, 0.2, 1.0);
	renderer->clear(sge::COLOR);
	renderer->begin();
	renderer->renderModels(gameObjects.size(), gameObjects.data());
	renderer->renderLights(pointLights.size(), pointLights.data());
	renderer->renderLights(dirLights.size(), dirLights.data());
	renderer->end();
	renderer->render();
	renderer->present();
	renderer->clear();
}

void Scene::interpolate(float alpha)
{

}

void Scene::loadTextShader(const std::string& path, std::vector<char>& data)
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

void Scene::loadBinaryShader(const std::string& path, std::vector<char>& data)
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

void Scene::mouseLook()
{
	if (firstMouse)
	{
		lastX += mouse.x;
		lastY += mouse.y;
		firstMouse = false;
	}

	mouseX += mouse.x;
	mouseY += mouse.y;

	float xoffset = mouseX - lastX;
	float yoffset = lastY - mouseY;
	lastX = static_cast<float>(mouseX);
	lastY = static_cast<float>(mouseY);

	float sensitivity = 0.15f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	sge::math::vec3 front;
	front.x = sge::math::cos(sge::math::radians(pitch)) * sge::math::cos(sge::math::radians(yaw));
	front.y = sge::math::sin(sge::math::radians(pitch));
	front.z = sge::math::cos(sge::math::radians(pitch)) * sge::math::sin(sge::math::radians(yaw));
	cameraFront = sge::math::normalize(front);
}