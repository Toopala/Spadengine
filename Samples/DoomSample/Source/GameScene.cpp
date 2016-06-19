#include "GameScene.h"
#include "Spade/Spade.h"
#include "Game/Entity.h"
#include "Renderer/Texture.h"
#include "Renderer/RenderTarget.h"
#include "Renderer/Enumerations.h"
#include "Renderer/Window.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/Pipeline.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexLayout.h"

#include "Core/Random.h"

/*
TODO


Tekstuuriresurssille tekstuurien generointi GPU:n muistiin
Sorttaus takasin kuntoon jos tarviijaksaahaluaa
Rendersystemille defaulttikamera!

*/

GameScene::GameScene(sge::Spade* engine) :
	engine(engine),
	renderer(engine->getRenderer()),
	useMouse(false),
	camSpeed(0.3f),
	alpha(0.0f),
	cubeX(0.5f),
	cubeY(1.0f),
	cubeZ(1.0f)
{
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

	vertexShader2 = engine->getRenderer()->getDevice()->createShader(sge::ShaderType::VERTEX, vShaderDataNormals.data(), vShaderDataNormals.size());
	pixelShader2 = engine->getRenderer()->getDevice()->createShader(sge::ShaderType::PIXEL, pShaderDataNormals.data(), pShaderDataNormals.size());

	pipelineNormals = engine->getRenderer()->getDevice()->createPipeline(&vertexLayoutDescription, vertexShader2, pixelShader2);
	engine->getRenderer()->getDevice()->bindPipeline(pipelineNormals);

	//// Systems ////
	physicsSystem = new sge::PhysicsSystem();
	
	//// Entities //// Creating entities without difficult components

	// Cube
	largeCube = entityManager.createEntity();
	modelEntities.push_back(largeCube);
	auto cubeTrans = transformFactory.create(largeCube);
	auto cubePhys = physicsSystem->createPhysicsComponent(largeCube);
	auto cubeModel = modelFactory.create(largeCube);

	// Room
	room = entityManager.createEntity();
	modelEntities.push_back(room);
	auto roomTrans = transformFactory.create(room);
	auto roomModel = modelFactory.create(room);

	// Camera
	camera = entityManager.createEntity();
	auto cameraTrans = transformFactory.create(camera);
	auto cameraComp = cameraFactory.create(camera);

	
	cameraPos = sge::math::vec3(0.0f, 0.0f, 5.0f);
	cameraFront = sge::math::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = sge::math::vec3(0.0f, 1.0f, 0.0f);

	cameraComp->setPerspective(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	cameraComp->setViewport(0, 0, 1280, 720);
	cameraTrans->setPosition(cameraPos);
	cameraTrans->setFront(cameraFront);
	cameraTrans->setUp(cameraUp);
	cameras.push_back(camera);

	//// Lights

	// Light 1

	pointLight1 = entityManager.createEntity();
	auto lightTrans1 = transformFactory.create(pointLight1);
	auto pLight1 = pLightFactory.create(pointLight1);

	lightTrans1->setPosition(sge::math::vec3(3.0f, 4.0f, 0.0f));

	sge::PointLight pointLightData1;
	pointLightData1.position = sge::math::vec4(0.0f);
	pointLightData1.constant = float(1.0);
	pointLightData1.mylinear = float(0.022);
	pointLightData1.quadratic = float(0.0019);
	pointLightData1.pad = 0.0f;
	pointLightData1.ambient = sge::math::vec4(0.0125, 0.05, 0.0125, 1.0);
	pointLightData1.diffuse = sge::math::vec4(0.2, 0.8, 0.2, 1.0);
	pointLightData1.specular = sge::math::vec4(0.25, 1.0, 0.25, 1.0);

	pLight1->setLightData(pointLightData1);

	pointLights.push_back(pointLight1);

	// Light 2

	pointLight2 = entityManager.createEntity();
	auto lightTrans2 = transformFactory.create(pointLight2);
	auto pLight2 = pLightFactory.create(pointLight2);

	lightTrans2->setPosition(sge::math::vec3(3.0f, 4.0f, 0.0f));

	sge::PointLight pointLightData2;
	pointLightData2.position = sge::math::vec4(0.0f);
	pointLightData2.constant = float(1.0);
	pointLightData2.mylinear = float(0.022);
	pointLightData2.quadratic = float(0.0019);
	pointLightData2.pad = 0.0f;
	pointLightData2.ambient = sge::math::vec4(0.05, 0.0125, 0.0125, 1.0);
	pointLightData2.diffuse = sge::math::vec4(0.8, 0.2, 0.2, 1.0);
	pointLightData2.specular = sge::math::vec4(1.0, 0.25, 0.25, 1.0);

	pLight2->setLightData(pointLightData2);

	// Light 3

	pointLight3 = entityManager.createEntity();
	auto lightTrans3 = transformFactory.create(pointLight3);
	auto pLight3 = pLightFactory.create(pointLight3);

	lightTrans3->setPosition(sge::math::vec3(3.0f, 4.0f, 0.0f));

	sge::PointLight pointLightData3;
	pointLightData3.position = sge::math::vec4(0.0f);
	pointLightData3.constant = float(1.0);
	pointLightData3.mylinear = float(0.022);
	pointLightData3.quadratic = float(0.0019);
	pointLightData3.pad = 0.0f;
	pointLightData3.ambient = sge::math::vec4(0.0125, 0.0125, 0.05, 1.0);
	pointLightData3.diffuse = sge::math::vec4(0.2, 0.2, 0.8, 1.0);
	pointLightData3.specular = sge::math::vec4(0.25, 0.25, 1.0, 1.0);

	pLight3->setLightData(pointLightData3);

	pointLights.push_back(pointLight3);

	//// Model handles ////

	// Cube

	cubeModelHandle = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/cubeSpecularNormal.dae");
	cubeModelHandle.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());

	// Room

	roomModelHandle = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/RoomBoxBig.dae");
	roomModelHandle.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());

	//// Free-for-all //// getters setters


	// Cube
	
	cubeModel->setShininess(15.0f);
	cubeModel->setModelResource(&cubeModelHandle);
	cubeTrans->setPosition(sge::math::vec3(0.0f, 0.0f, 0.0f));
	cubeTrans->setRotationVector(sge::math::vec3(0.0f, 0.0f, 1.0f));
	cubeModel->setPipeline(pipelineNormals);
	cubeModelHandle.getResource<sge::ModelResource>()->createBuffers();
	
	// Room

	roomModel->setShininess(250.0f);
	roomModel->setModelResource(&roomModelHandle);
	roomTrans->setPosition(sge::math::vec3(5.0f, 0.0f, 0.0f));
	roomTrans->setRotationVector(sge::math::vec3(1.0f, 0.0f, 0.0f));
	roomTrans->setAngle(sge::math::radians(-90.0f));
	roomTrans->setScale(sge::math::vec3(0.6f));
	roomModel->setPipeline(pipelineNormals);
	roomModelHandle.getResource<sge::ModelResource>()->createBuffers();

	//// Bullet ////

	physicsSystem->getWorld()->setGravity(btVector3(0, 0, 0)); // No gravity

	btScalar mass = 1;
	boxShape = new btBoxShape(btVector3(1, 1, 1));
	cubePhys->createBody(boxShape, btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0), mass, btVector3(0, 0, 0));
	physicsSystem->addBody(cubePhys->getBody<btRigidBody>());
}

GameScene::~GameScene()
{
	sge::ResourceManager::getMgr().release(cubeModelHandle);
	sge::ResourceManager::getMgr().release(roomModelHandle);
	engine->getRenderer()->getDevice()->debindPipeline(pipelineNormals);
	delete physicsSystem; // Destructor of physicsSystem handles deletion of rigidbodies
}

void GameScene::loadTextShader(const std::string& path, std::vector<char>& data)
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

void GameScene::loadBinaryShader(const std::string& path, std::vector<char>& data)
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


void GameScene::update(float step)
{
	//// Camera Controls ////
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

	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_SPACE))
	{
		if (useMouse == true)
		{
			sge::math::vec3 temp = cameras[0]->getComponent<sge::TransformComponent>()->getPosition();
			sge::math::vec3 frontTemp = cameras[0]->getComponent<sge::TransformComponent>()->getFront();
			sge::math::vec3 upTemp = cameras[0]->getComponent<sge::TransformComponent>()->getUp();
			cameras[0]->getComponent<sge::TransformComponent>()->setPosition(temp + upTemp*camSpeed);
		}
	}

	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_LCTRL))
	{
		if (useMouse == true)
		{
			sge::math::vec3 temp = cameras[0]->getComponent<sge::TransformComponent>()->getPosition();
			sge::math::vec3 frontTemp = cameras[0]->getComponent<sge::TransformComponent>()->getFront();
			sge::math::vec3 upTemp = cameras[0]->getComponent<sge::TransformComponent>()->getUp();
			cameras[0]->getComponent<sge::TransformComponent>()->setPosition(temp - upTemp*camSpeed);
		}
	}

	if (useMouse)
	{
#ifdef _WIN32
		engine->mouseInput->getRelativeMouseState(&mouseXpos, &mouseYpos);

		mouseLook(mouseXpos, mouseYpos);
#endif
		cameras[0]->getComponent<sge::TransformComponent>()->setFront(cameraFront);
	}

	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_ESCAPE))
	{
		engine->stop();
	}

	///////////////

	alpha += 0.01f;
	float x = 15.0f*cos(alpha);
	float z = 15.0f*sin(alpha);
	
	
	largeCube->getComponent<sge::PhysicsComponent>()->getBody<btRigidBody>()->setAngularVelocity(
		btVector3(cubeX, cubeY, cubeZ));


	physicsSystem->stepWorld(step);
	physicsSystem->update();

	pointLight1->getComponent<sge::TransformComponent>()->setPosition(sge::math::vec3(x, 4.0f, z));
	pointLight1->getComponent<sge::PointLightComponent>()->update();

	pointLight2->getComponent<sge::TransformComponent>()->setPosition(sge::math::vec3(z, 5.0f, x));
	pointLight2->getComponent<sge::PointLightComponent>()->update();

	pointLight3->getComponent<sge::TransformComponent>()->setPosition(sge::math::vec3(-z, 6.0f, -x));
	pointLight3->getComponent<sge::PointLightComponent>()->update();

	

	for (auto &camera : cameras)
	{
		camera->getComponent<sge::CameraComponent>()->update();
	}
}

void GameScene::interpolate(float alpha)
{

}

void GameScene::draw()
{
    // Note that we need to set render targets and cameras before we begin.
    // First pass
	renderer->addCameras(1, &cameras.front());

    renderer->begin();
    

	renderer->renderModels(modelEntities.size(), modelEntities.data());

	renderer->renderLights(1, &pointLight1);
	renderer->renderLights(1, &pointLight2);
	renderer->renderLights(1, &pointLight3);
	
    renderer->end();

    renderer->render();

    // Second pass

	//renderer->setClearColor(0.0f, 0.0f, 0.5f, 1.0f);
    //renderer->clear();
	// renderer->render();
	
    // Present to screen
    renderer->present();
	renderer->setClearColor(0.0f, 0.0f, 0.5f, 1.0f);
    renderer->clear();
}

void GameScene::mouseLook(int mouseX, int mouseY)
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
