#include "VisualScene.h"

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

VisualScene::VisualScene(sge::Spade *engine)
	: engine(engine)
	, renderer(engine->getRenderer())
{
	std::vector<char> pShaderData;
	std::vector<char> vShaderData;

	// Audio test
	// sge::Audio mixer;

#ifdef DIRECTX11
	loadBinaryShader("../../Shaders/Compiled/VertexShaderLights.cso", vShaderData);
	loadBinaryShader("../../Shaders/Compiled/PixelShaderLights.cso", pShaderData);
#elif OPENGL4
	loadTextShader("../Assets/Shaders/VertexShaderLightsNoNormalTexture.glsl", vShaderData);
	loadTextShader("../Assets/Shaders/PixelShaderLightsNoNormalTexture.glsl", pShaderData);
#endif

	sge::VertexLayoutDescription vertexLayoutDescription = { 5,
	{
		{ 0, 3, sge::VertexSemantic::POSITION },
		{ 0, 3, sge::VertexSemantic::NORMAL },
		{ 0, 3, sge::VertexSemantic::TANGENT },
		{ 0, 3, sge::VertexSemantic::TANGENT },
		{ 0, 2, sge::VertexSemantic::TEXCOORD }
	} };


	vertexShader = engine->getRenderer()->getDevice()->createShader(sge::ShaderType::VERTEX, vShaderData.data(), vShaderData.size());
	pixelShader = engine->getRenderer()->getDevice()->createShader(sge::ShaderType::PIXEL, pShaderData.data(), pShaderData.size());

	pipeline = engine->getRenderer()->getDevice()->createPipeline(&vertexLayoutDescription, vertexShader, pixelShader);
	engine->getRenderer()->getDevice()->bindPipeline(pipeline);

	//--------------
	// New pipeline
	std::vector<char> pShaderDataNormals;
	std::vector<char> vShaderDataNormals;

#ifdef DIRECTX11
	loadBinaryShader("../../Shaders/Compiled/VertexShaderLights.cso", vShaderDataNormals);
	loadBinaryShader("../../Shaders/Compiled/PixelShaderLights.cso", pShaderDataNormals);
#elif OPENGL4
	loadTextShader("../Assets/Shaders/VertexShaderLights.glsl", vShaderDataNormals);
	loadTextShader("../Assets/Shaders/PixelShaderLights.glsl", pShaderDataNormals);
#endif

	vertexShader2 = engine->getRenderer()->getDevice()->createShader(sge::ShaderType::VERTEX, vShaderDataNormals.data(), vShaderData.size());
	pixelShader2 = engine->getRenderer()->getDevice()->createShader(sge::ShaderType::PIXEL, pShaderDataNormals.data(), pShaderData.size());

	pipelineNormals = engine->getRenderer()->getDevice()->createPipeline(&vertexLayoutDescription, vertexShader2, pixelShader2);
	engine->getRenderer()->getDevice()->bindPipeline(pipelineNormals);

	// ----------------------------------------------------

	// Assimp models
	modelHandle = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/cubeSpecularNormal.dae");
	modelHandle.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());

	// Create new entity for model
	EManager = new sge::EntityManager();

	modentity = EManager->createEntity();

	modtransform = new sge::TransformComponent(modentity);
	modentity->setComponent(modtransform);

	modcomponent = new sge::ModelComponent(modentity);
	modcomponent->setShininess(500.0f);
	modentity->setComponent(modcomponent);

	modcomponent->setModelResource(&modelHandle);
	modcomponent->setRenderer(engine->getRenderer());

	modentity->getComponent<sge::TransformComponent>()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	modentity->getComponent<sge::TransformComponent>()->setRotationVector(glm::vec3(0.0f, 0.0f, 1.0f));

	modcomponent->setPipeline(pipeline);

	modelHandle.getResource<sge::ModelResource>()->createBuffers();

	// Camera

	sge::Entity* camentity = EManager->createEntity();

	camtransform = new sge::TransformComponent(camentity);
	camentity->setComponent(camtransform);

	camcomponent = new sge::CameraComponent(camentity);
	camentity->setComponent(camcomponent);

	cameraPos = glm::vec3(5.0f, 10.0f, 50.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	camcomponent->setPerspective(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	camcomponent->setViewport(0, 0, 1280, 720);
	camtransform->setPosition(cameraPos);
	camtransform->setFront(cameraFront);
	camtransform->setUp(cameraUp);

	cameras.push_back(camentity);
}

void VisualScene::update(float step)
{

	modentity->getComponent<sge::TransformComponent>()->setPosition(sge::math::vec3(5.0f, 5.0f, 30.0f));
	//modentity->getComponent<sge::TransformComponent>()->setAngle(0.0f);
	//modentity->getComponent<sge::TransformComponent>()->setRotationVector(sge::math::vec3(trans.getRotation().getAxis().getX(), trans.getRotation().getAxis().getY(), trans.getRotation().getAxis().getZ()));
	
	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_ESCAPE))
	{
		engine->stop();
	}

	for (auto camera : cameras)
	{
		camera->getComponent<sge::CameraComponent>()->update();
	}
}

void VisualScene::interpolate(float alpha)
{

}

void VisualScene::draw()
{
	renderer->addCameras(1, &cameras.front());
	renderer->setClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	renderer->clear(sge::COLOR);
	renderer->begin();
	renderer->renderModels(1, &modentity);
	renderer->end();

	renderer->render();

	renderer->present();
	renderer->clear();
}

void VisualScene::loadTextShader(const std::string& path, std::vector<char>& data)
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

void VisualScene::loadBinaryShader(const std::string& path, std::vector<char>& data)
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

VisualScene::~VisualScene()
{
	delete
		  engine
		, renderer
		, EManager
		, modentity
		, modelHandle
		, modcomponent
		, modtransform;

	sge::ResourceManager::getMgr().release(modelHandle);

	engine->getRenderer()->getDevice()->debindPipeline(pipeline);


	engine->getRenderer()->getDevice()->deleteShader(vertexShader);
	engine->getRenderer()->getDevice()->deleteShader(pixelShader);

	engine->getRenderer()->getDevice()->deletePipeline(pipeline);
}