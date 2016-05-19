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
    renderer(engine->getRenderer())
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
	largeCube->setComponent(transformFactory.create(largeCube));
	largeCube->setComponent(physicsSystem->createPhysicsComponent(largeCube));
	largeCube->setComponent(modelFactory.create(largeCube));

	// Camera
	camera = entityManager.createEntity();
	camera->setComponent(transformFactory.create(camera));
	camera->setComponent(cameraFactory.create(camera));
	cameraPos = glm::vec3(5.0f, 10.0f, 48.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	camera->getComponent<sge::CameraComponent>()->setPerspective(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	camera->getComponent<sge::CameraComponent>()->setViewport(1280 - 420, 720 - 280, 320, 180);
	camera->getComponent<sge::TransformComponent>()->setPosition(cameraPos);
	camera->getComponent<sge::TransformComponent>()->setFront(cameraFront);
	camera->getComponent<sge::TransformComponent>()->setUp(cameraUp);
	cameras.push_back(camera);

	// sge::TransformComponent* cubeTrans = largeCube->getComponent<sge::TransformComponent>();

	//// Model handles ////
	modelHandle = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/cubeSpecularNormal.dae");
	modelHandle.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());

	//// Free-for-all //// getters setters

	largeCube->getComponent<sge::ModelComponent>()->setShininess(15.0f);
	largeCube->getComponent<sge::ModelComponent>()->setModelResource(&modelHandle);
	largeCube->getComponent<sge::ModelComponent>()->setRenderer(engine->getRenderer());
	largeCube->getComponent<sge::TransformComponent>()->setPosition(glm::vec3(0.0f, 23.0f, 0.0f));
	largeCube->getComponent<sge::TransformComponent>()->setRotationVector(glm::vec3(0.0f, 0.0f, 1.0f));
	largeCube->getComponent<sge::ModelComponent>()->setPipeline(pipelineNormals);
	modelHandle.getResource<sge::ModelResource>()->createBuffers();
	




	//// Bullet ////
	btScalar mass = 1;
	boxShape = new btBoxShape(btVector3(1, 1, 1));
	largeCube->getComponent<sge::PhysicsComponent>()->createBody(boxShape, btQuaternion(0, 0, 0, 1), btVector3(5, 20, 0), mass, btVector3(0, 0, 0));
	physicsSystem->addBody(largeCube->getComponent<sge::PhysicsComponent>()->getBody<btRigidBody>());
}

GameScene::~GameScene()
{ // DEBUG ASSERTION FAILURE
	/*physicsSystem->getWorld()->removeRigidBody(largeCube->getComponent<sge::PhysicsComponent>()->getBody<btRigidBody>());
	delete largeCube->getComponent<sge::PhysicsComponent>()->getBody<btRigidBody>()->getMotionState();
	delete largeCube->getComponent<sge::PhysicsComponent>()->getBody<btRigidBody>();
	sge::ResourceManager::getMgr().release(modelHandle);
	engine->getRenderer()->getDevice()->debindPipeline(pipelineNormals);
	delete physicsSystem;

	for (auto &camera : cameras)
	{
		delete camera;
	}
	cameras.clear();*/
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
	physicsSystem->stepWorld(step);

	// Inputs and stuffs
}

void GameScene::interpolate(float alpha)
{

}

void GameScene::draw()
{
    // Note that we need to set render targets and cameras before we begin.
    // First pass
	// renderer->addCameras(1, &cameras.front());

    renderer->begin();
    
	// !!!!! Fix this !!!!!
	//renderer->renderModel(largeCube->getComponent<sge::ModelComponent>());

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



//sge::Entity* GameScene::createCamera(int x, int y, unsigned int width, unsigned int height)
//{
//    // TODO cameracomponent doesn't use transform component and directly
//    // requests input from (old design) spade singleton. These should be fixed.
//    // Input system needs more planning to do.
//    sge::Entity* entity = entityManager.createEntity();
//
//    auto transform = transformFactory.create(entity);
//    auto cameracomponent = cameraFactory.create(entity);
//
//    transform->setPosition({ 0.0f, 0.0f, 10.0f });
//    transform->setFront({ 0.0f, 0.0f, -1.0f });
//    transform->setUp({ 0.0f, 1.0f, 0.0f });
//
//    cameracomponent->setOrtho(0.0f, (float)width, (float)height, 0.0f, 0.1f, 1000.0f);
//    cameracomponent->setViewport(x, y, width, height);
//
//    return entity;
//}

//sge::Entity* GameScene::createText(float x, float y, const std::string& text)
//{
//    sge::Entity* entity = entityManager.createEntity();
//
//    auto transform = transformFactory.create(entity);
//    auto textcomponent = textFactory.create(entity);
//    
//    transform->setPosition({ x, y, 0.0f });
//
//    textcomponent->setColor({ 1.0f, 1.0f, 1.0f, 1.0f });
//    textcomponent->setFont(fontResource.getResource<sge::FontResource>()->getFont());
//    textcomponent->setText(text);
//
//    return entity;
//}