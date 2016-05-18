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
	/*

	// Create entity manager:
	entityManager = new sge::EntityManager();
	// ----------------------

	// Shader:
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

	// Light:
	sge::PointLight pointLight;
	pointLight.position = sge::math::vec4(0);
	pointLight.constant = 1;
	pointLight.mylinear = 0.09;
	pointLight.quadratic = 0.0032;
	pointLight.pad = 0;
	pointLight.ambient = sge::math::vec4(0.05, 0.05, 0.05, 1);
	pointLight.diffuse = sge::math::vec4(3, 3, 3, 1);
	pointLight.specular = sge::math::vec4(1, 1, 1, 1);

	pointLightEntity = entityManager->createEntity();
	pointLightTransform = new sge::TransformComponent(pointLightEntity);
	pointLightTransform->setPosition({ 3, 4, 0 });
	pointLightEntity->setComponent(pointLightTransform);
	pointLightComponent = new sge::PointLightComponent(pointLightEntity);
	pointLightComponent->setLightData(pointLight);
	pointLightEntity->setComponent(pointLightComponent);
	// ----------------------

	// Camera:
	sge::Entity* camera = entityManager->createEntity();

	cameraTransform = new sge::TransformComponent(camera);
	cameraTransform->setPosition({ 0, 0, -5 });
	cameraTransform->setFront({ 0, 0, -5 });
	cameraTransform->setUp({ 0, 1, 0 });
	camera->setComponent(cameraTransform);

	cameraComponent = new sge::CameraComponent(camera);
	cameraComponent->setPerspective(45, 16/9, 0.1, 1000);
	cameraComponent->setViewport(0, 0, 1280, 720);
	camera->setComponent(cameraComponent);

	cameras.push_back(camera);
	// ----------------------

	// Cube model
	cubeModelHandle = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/cubeSpecularNormal.dae");
	cubeModelHandle.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());
	cubeModel = entityManager->createEntity();

	cubeTransform = new sge::TransformComponent(cubeModel);
	cubeTransform->setPosition({ 0, 0, -100 });
	cubeTransform->setRotationVector({ 0, 0, 1 });
	cubeTransform->setScale({10, 10, 10});
	cubeModel->setComponent(cubeTransform);

	cubeComponent = new sge::ModelComponent(cubeModel);
	cubeComponent->setShininess(250);
	cubeComponent->setModelResource(&cubeModelHandle);
	cubeComponent->setRenderer(engine->getRenderer());
	cubeComponent->setPipeline(pipeline);
	cubeModel->setComponent(cubeComponent);

	cubeModelHandle.getResource<sge::ModelResource>()->createBuffers();
	gameObjects.push_back(cubeModel);
	// ----------------------

	*/
}

Scene::~Scene()
{
	sge::ResourceManager::getMgr().release(cubeModelHandle);
	engine->getRenderer()->getDevice()->debindPipeline(pipeline);
	engine->getRenderer()->getDevice()->deleteShader(vertexShader);
	engine->getRenderer()->getDevice()->deleteShader(pixelShader);
}

void Scene::update(float dt)
{
	// Mouse inputs:
	engine->mouseInput->getRelativeMouseState(&mouse.x, &mouse.y);

	// Keyboard inputs:
	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_ESCAPE))
	{
		engine->stop();
	}

	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_W))
	{
		cameras[0]->getComponent<sge::TransformComponent>()->addPosition({0, -5 ,0 });
	}
	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_S))
	{
		cameras[0]->getComponent<sge::TransformComponent>()->addPosition({ 0, 5, 0 });
	}
	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_A))
	{
		cameras[0]->getComponent<sge::TransformComponent>()->addPosition({ 5, 0, 0 });
	}
	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_D))
	{
		cameras[0]->getComponent<sge::TransformComponent>()->addPosition({ -5, 0, 0 });
	}

	// Camera update:
	for (auto camera : cameras)
	{
		camera->getComponent<sge::CameraComponent>()->update();
	}
}

void Scene::draw()
{
	//renderer->addCameras(1, &cameras.front());
	renderer->setClearColor(0.0, 0.5, 0.0, 1.0);
	renderer->clear(sge::COLOR);
	renderer->begin();
	//renderer->renderModels(gameObjects.size(), gameObjects.data());
	//renderer->renderLights(1, &pointLightEntity);
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