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

VisualScene::VisualScene(sge::Spade *engine)
	: engine(engine)
	, renderer(engine->getRenderer())
	, useMouse(false)
	, camSpeed(1.0f)
{
	sge::VertexLayoutDescription vertexLayoutDescription = { 5,
	{
		{ 0, 3, sge::VertexSemantic::POSITION },
		{ 0, 3, sge::VertexSemantic::NORMAL },
		{ 0, 3, sge::VertexSemantic::TANGENT },
		{ 0, 3, sge::VertexSemantic::TANGENT },
		{ 0, 2, sge::VertexSemantic::TEXCOORD }
	} };

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

	vertexShader2 = engine->getRenderer()->getDevice()->createShader(sge::ShaderType::VERTEX, vShaderDataNormals.data(), vShaderDataNormals.size());
	pixelShader2 = engine->getRenderer()->getDevice()->createShader(sge::ShaderType::PIXEL, pShaderDataNormals.data(), pShaderDataNormals.size());

	pipeline = engine->getRenderer()->getDevice()->createPipeline(&vertexLayoutDescription, vertexShader2, pixelShader2);
	engine->getRenderer()->getDevice()->bindPipeline(pipeline);

	// ----------------------------------------------------

	// Assimp models
	modelHandleCube = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/saber.dae");
	modelHandleCube.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());

	modelHandleCube2 = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/cubeSpecularNormal.dae");
	modelHandleCube2.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());

	modelHandleCube3 = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/simpleCube.dae");
	modelHandleCube3.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());

	modelHandleRoom = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/RoomBoxBig2.dae");
	modelHandleRoom.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());

	// Create new entity for models
	// Cube 1
	EManager = new sge::EntityManager();

	modentityCube = EManager->createEntity();

	modtransformCube = new sge::TransformComponent(modentityCube);
	modentityCube->setComponent(modtransformCube);

	modComponentCube = new sge::ModelComponent(modentityCube);
	modComponentCube->setShininess(50.0f);
	modComponentCube->setGlossyness(1.0f);

	sge::Handle<sge::TextureResource> tex1;
	sge::Handle<sge::TextureResource> tex2;
	sge::Handle<sge::TextureResource> tex3;
	sge::Handle<sge::TextureResource> tex4;
	sge::Handle<sge::TextureResource> tex5;
	sge::Handle<sge::TextureResource> tex6;
	tex1 = sge::ResourceManager::getMgr().load<sge::TextureResource>("../Assets/CubeMap/right.jpg");
	tex2 = sge::ResourceManager::getMgr().load<sge::TextureResource>("../Assets/CubeMap/left.jpg");
	tex3 = sge::ResourceManager::getMgr().load<sge::TextureResource>("../Assets/CubeMap/top.jpg");
	tex4 = sge::ResourceManager::getMgr().load<sge::TextureResource>("../Assets/CubeMap/bottom.jpg");
	tex5 = sge::ResourceManager::getMgr().load<sge::TextureResource>("../Assets/CubeMap/back.jpg");
	tex6 = sge::ResourceManager::getMgr().load<sge::TextureResource>("../Assets/CubeMap/front.jpg");

    sge::TextureResource* source[6];
	source[5] = tex1.getResource<sge::TextureResource>();
	source[4] = tex2.getResource<sge::TextureResource>();
	source[3] = tex3.getResource<sge::TextureResource>();
	source[2] = tex4.getResource<sge::TextureResource>();
	source[1] = tex5.getResource<sge::TextureResource>();
	source[0] = tex6.getResource<sge::TextureResource>();
	modComponentCube->setCubeMap(engine->getRenderer()->getDevice()->createCubeMap(source));
	modentityCube->setComponent(modComponentCube);

	modComponentCube->setModelResource(&modelHandleCube);
	modComponentCube->setRenderer(engine->getRenderer());

	modentityCube->getComponent<sge::TransformComponent>()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	modentityCube->getComponent<sge::TransformComponent>()->setRotationVector(glm::vec3(0.0f, 0.0f, 1.0f));
	modentityCube->getComponent<sge::TransformComponent>()->setScale(glm::vec3(2.0f));

	modComponentCube->setPipeline(pipeline);

	modelHandleCube.getResource<sge::ModelResource>()->createBuffers();

	// Cube 2
	modentityCube2 = EManager->createEntity();

	modtransformCube2 = new sge::TransformComponent(modentityCube2);
	modentityCube2->setComponent(modtransformCube2);

	modComponentCube2 = new sge::ModelComponent(modentityCube2);
	modComponentCube2->setShininess(15.0f);
	modentityCube2->setComponent(modComponentCube2);

	modComponentCube2->setModelResource(&modelHandleCube2);
	modComponentCube2->setRenderer(engine->getRenderer());

	modentityCube2->getComponent<sge::TransformComponent>()->setPosition(glm::vec3(20.0f, 0.0f, 0.0f));
	modentityCube2->getComponent<sge::TransformComponent>()->setRotationVector(glm::vec3(0.0f, 0.0f, 1.0f));
	modentityCube2->getComponent<sge::TransformComponent>()->setScale(glm::vec3(5));

	modComponentCube2->setPipeline(pipeline);

	modelHandleCube2.getResource<sge::ModelResource>()->createBuffers();

	// Cube 3
	modentityCube3 = EManager->createEntity();

	modtransformCube3 = new sge::TransformComponent(modentityCube3);
	modentityCube3->setComponent(modtransformCube3);

	modComponentCube3 = new sge::ModelComponent(modentityCube3);
	modComponentCube3->setShininess(50.0f);
	modComponentCube3->setGlossyness(1.0f);

	sge::TextureResource* source2[6];
	source2[5] = tex1.getResource<sge::TextureResource>();
	source2[4] = tex2.getResource<sge::TextureResource>();
	source2[3] = tex3.getResource<sge::TextureResource>();
	source2[2] = tex4.getResource<sge::TextureResource>();
	source2[1] = tex5.getResource<sge::TextureResource>();
	source2[0] = tex6.getResource<sge::TextureResource>();
	modComponentCube3->setCubeMap(engine->getRenderer()->getDevice()->createCubeMap(source2));
	modentityCube3->setComponent(modComponentCube3);

	modComponentCube3->setModelResource(&modelHandleCube3);
	modComponentCube3->setRenderer(engine->getRenderer());

	modentityCube3->getComponent<sge::TransformComponent>()->setPosition(glm::vec3(-20.0f, 0.0f, 0.0f));
	modentityCube3->getComponent<sge::TransformComponent>()->setRotationVector(glm::vec3(0.0f, 0.0f, 1.0f));
	modentityCube3->getComponent<sge::TransformComponent>()->setScale(glm::vec3(5));

	modComponentCube3->setPipeline(pipeline);

	modelHandleCube3.getResource<sge::ModelResource>()->createBuffers();

	// Room for scene
	modentityRoom = EManager->createEntity();
	
	modtransformRoom = new sge::TransformComponent(modentityRoom);
	modentityRoom->setComponent(modtransformRoom);

	modComponentRoom = new sge::ModelComponent(modentityRoom);
	modComponentRoom->setShininess(250.0f);
	modentityRoom->setComponent(modComponentRoom);

	modComponentRoom->setModelResource(&modelHandleRoom);
	modComponentRoom->setRenderer(engine->getRenderer());

	modentityRoom->getComponent<sge::TransformComponent>()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	modentityRoom->getComponent<sge::TransformComponent>()->setRotationVector(glm::vec3(1.0f, 0.0f, 0.0f));
	modentityRoom->getComponent<sge::TransformComponent>()->setAngle(sge::math::radians(-90.0f));
	modentityRoom->getComponent<sge::TransformComponent>()->setScale(glm::vec3(0.6f));

	modComponentRoom->setPipeline(pipeline);
	modelHandleRoom.getResource<sge::ModelResource>()->createBuffers();

	// Push back game entities
	gameObjects.push_back(modentityCube);
	gameObjects.push_back(modentityCube2);
	gameObjects.push_back(modentityCube3);
	gameObjects.push_back(modentityRoom);

	// Lights
	modentityLight = EManager->createEntity();
	modtransformLight = new sge::TransformComponent(modentityLight);
	modentityLight->setComponent(modtransformLight);
	modentityLight->getComponent<sge::TransformComponent>()->setPosition(glm::vec3(3.0f, 5.0f, 9.0f));
	pointLightComp = new sge::PointLightComponent(modentityLight);
	modentityLight->setComponent(pointLightComp);

	sge::PointLight pointLight;
	pointLight.position = sge::math::vec4(0.0f);
	pointLight.constant = float(1.0);
	pointLight.mylinear = float(0.09);
	pointLight.quadratic = float(0.0032);
	pointLight.pad = 0.0f;
	pointLight.ambient = sge::math::vec4(0.1f, 0.1f, 0.1f, 1.0);
	pointLight.diffuse = sge::math::vec4(5.0f, 5.0f, 5.0f, 1.0);
	pointLight.specular = sge::math::vec4(1.0f, 1.0, 1.0f, 1.0);

	modentityLight->getComponent<sge::PointLightComponent>()->setLightData(pointLight);

	modentityLight2 = EManager->createEntity();
	modtransformLight2 = new sge::TransformComponent(modentityLight2);
	modentityLight2->setComponent(modtransformLight2);
	modentityLight2->getComponent<sge::TransformComponent>()->setPosition(glm::vec3(3.0f, 5.0f, 9.0f));
	pointLightComp2 = new sge::PointLightComponent(modentityLight2);
	modentityLight2->setComponent(pointLightComp2);

	sge::PointLight pointLight2;
	pointLight2.position = sge::math::vec4(0.0f);
	pointLight2.constant = float(1.0);
	pointLight2.mylinear = float(0.09);
	pointLight2.quadratic = float(0.0032);
	pointLight2.pad = 0.0f;
	pointLight2.ambient = sge::math::vec4(0.1f, 0.1f, 0.1f, 1.0);
	pointLight2.diffuse = sge::math::vec4(5.0f, 5.0f, 5.0f, 1.0);
	pointLight2.specular = sge::math::vec4(1.0f, 1.0, 1.0f, 1.0);

	modentityLight2->getComponent<sge::PointLightComponent>()->setLightData(pointLight2);

	// Camera
	sge::Entity* camentity = EManager->createEntity();

	camtransform = new sge::TransformComponent(camentity);
	camentity->setComponent(camtransform);

	camcomponent = new sge::CameraComponent(camentity);
	camentity->setComponent(camcomponent);

	cameraPos = glm::vec3(0.0f, 0.0f, -5.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -5.0f);
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
	updateControls();

	alpha += 0.02f;
	rotate += sge::math::radians(0.5f);

	modentityCube->getComponent<sge::TransformComponent>()->setRotationVector(glm::vec3(1.0f, 0.0f, 1.0f));
	modentityCube->getComponent<sge::TransformComponent>()->setAngle(rotate);
	modentityCube2->getComponent<sge::TransformComponent>()->setRotationVector(glm::vec3(1.0f, 0.0f, 1.0f));
	modentityCube2->getComponent<sge::TransformComponent>()->setAngle(-rotate);
	//modentityCube3->getComponent<sge::TransformComponent>()->setAngle(rotate);

	float lightX = -25.0f*cos(alpha);
	float lightY = -25.0f*sin(alpha);
	float lightZ = -25.0f*sin(alpha);

	modentityLight->getComponent<sge::TransformComponent>()->setPosition(sge::math::vec3(lightX, lightY, lightZ));
	modentityLight->getComponent<sge::PointLightComponent>()->update();
	modentityLight2->getComponent<sge::TransformComponent>()->setPosition(sge::math::vec3(lightX, lightY, lightZ) * glm::vec3(-1));
	modentityLight2->getComponent<sge::PointLightComponent>()->update();


	for (auto camera : cameras)
	{
		camera->getComponent<sge::CameraComponent>()->update();
	}
}


void VisualScene::draw()
{
	renderer->addCameras(1, &cameras.front());
	renderer->setClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	renderer->clear(sge::COLOR);
	renderer->begin();
	
	// Render all game objects
	renderer->renderModels(gameObjects.size(), gameObjects.data());

	// Render lights
	renderer->renderLights(1, &modentityLight);
	renderer->renderLights(1, &modentityLight2);

	renderer->end();

	renderer->render();

	renderer->present();
	renderer->clear();
}

void VisualScene::updateControls()
{


	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_1))
	{
		useMouse = true;
		if (useMouse) SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_2))
	{
		useMouse = false;
		if (!useMouse) SDL_SetRelativeMouseMode(SDL_FALSE);
	}

	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_W))
	{
		if (useMouse == true)
		{
			sge::math::vec3 temp = cameras[0]->getComponent<sge::TransformComponent>()->getPosition();
			cameras[0]->getComponent<sge::TransformComponent>()->setPosition(temp + cameraFront*camSpeed);
		}
	}

	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_RETURN) && player == false)
	{
		audio.play("../Assets/Audio/person_cheering.wav");
		player = true;
	}

	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_SPACE) && player == true)
	{
		audio.stop();
		player = false;
	}

	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_S))
	{
		if (useMouse == true)
		{
			sge::math::vec3 temp = cameras[0]->getComponent<sge::TransformComponent>()->getPosition();
			cameras[0]->getComponent<sge::TransformComponent>()->setPosition(temp - cameraFront*camSpeed);
		}
	}
	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_A))
	{
		if (useMouse == true)
		{
			sge::math::vec3 temp = cameras[0]->getComponent<sge::TransformComponent>()->getPosition();
			sge::math::vec3 frontTemp = cameras[0]->getComponent<sge::TransformComponent>()->getFront();
			sge::math::vec3 upTemp = cameras[0]->getComponent<sge::TransformComponent>()->getUp();
			cameras[0]->getComponent<sge::TransformComponent>()->setPosition(temp - sge::math::cross(frontTemp, upTemp)*camSpeed);
		}
	}
	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_D))
	{
		if (useMouse == true)
		{
			sge::math::vec3 temp = cameras[0]->getComponent<sge::TransformComponent>()->getPosition();
			sge::math::vec3 frontTemp = cameras[0]->getComponent<sge::TransformComponent>()->getFront();
			sge::math::vec3 upTemp = cameras[0]->getComponent<sge::TransformComponent>()->getUp();
			cameras[0]->getComponent<sge::TransformComponent>()->setPosition(temp + sge::math::cross(frontTemp, upTemp)*camSpeed);
		}
	}

	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_ESCAPE))
	{
		engine->stop();
	}

	if (useMouse)
	{
#ifdef _WIN32
		engine->mouseInput->getRelativeMouseState(&mouseXpos, &mouseYpos);

		mouseLook(mouseXpos, mouseYpos);
#endif
		cameras[0]->getComponent<sge::TransformComponent>()->setFront(cameraFront);
	}


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

void VisualScene::mouseLook(int mouseX, int mouseY)
{
	if (firstMouse)
	{
		lastX += mouseX;
		lastY += mouseY;
		firstMouse = false;
	}

	mousseX += mouseX;
	mousseY += mouseY;

	float xoffset = mousseX - lastX;
	float yoffset = lastY - mousseY;
	lastX = static_cast<float>(mousseX);
	lastY = static_cast<float>(mousseY);

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


void VisualScene::interpolate(float alpha)
{

}

VisualScene::~VisualScene()
{
	sge::ResourceManager::getMgr().release(modelHandleCube);
	sge::ResourceManager::getMgr().release(modelHandleCube2);
	sge::ResourceManager::getMgr().release(modelHandleCube3);
	sge::ResourceManager::getMgr().release(modelHandleRoom);

	engine->getRenderer()->getDevice()->debindPipeline(pipeline);
}