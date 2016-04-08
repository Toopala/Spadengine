#include "BulletTestScene.h"

#include "stb_image.h"

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

void BulletTestScene::loadTextShader(const std::string& path, std::vector<char>& data)
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

void BulletTestScene::loadBinaryShader(const std::string& path, std::vector<char>& data)
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

BulletTestScene::BulletTestScene()
{
	modelSystem = new sge::ModelRenderingSystem(sge::Spade::getInstance().getRenderer());
	std::vector<char> pShaderData;
	std::vector<char> vShaderData;

#ifdef DIRECTX11
	loadBinaryShader("../Assets/Shaders/VertexShader.cso", vShaderData);
	loadBinaryShader("../Assets/Shaders/PixelShader.cso", pShaderData);
#elif OPENGL4
	loadTextShader("../Assets/Shaders/VertexShader.glsl", vShaderData);
	loadTextShader("../Assets/Shaders/PixelShader.glsl", pShaderData);
#endif

	sge::VertexLayoutDescription vertexLayoutDescription = { 5,
	{
		{ 0, 3, sge::VertexSemantic::POSITION },
		{ 0, 3, sge::VertexSemantic::NORMAL },
		{ 0, 3, sge::VertexSemantic::TANGENT },
		{ 0, 3, sge::VertexSemantic::TANGENT },
		{ 0, 2, sge::VertexSemantic::TEXCOORD }
	} };

	vertexShader = sge::Spade::getInstance().getRenderer()->getDevice()->createShader(sge::ShaderType::VERTEX, vShaderData.data(), vShaderData.size());
	pixelShader = sge::Spade::getInstance().getRenderer()->getDevice()->createShader(sge::ShaderType::PIXEL, pShaderData.data(), pShaderData.size());

	pipeline = sge::Spade::getInstance().getRenderer()->getDevice()->createPipeline(&vertexLayoutDescription, vertexShader, pixelShader);
	sge::Spade::getInstance().getRenderer()->getDevice()->bindPipeline(pipeline);

	//Assimp test
	modelHandle = sge::Spade::getInstance().getResourceManager()->load<sge::ModelResource>("../Assets/suzanne.dae");
	modelHandle.getResource<sge::ModelResource>()->setRenderer(sge::Spade::getInstance().getRenderer());

	EManager = new sge::EntityManager();
	sysManager = new sge::SystemManager();
	sysManager->init();
	EManager->setSysManager(sysManager);
	modentity = EManager->createEntity();

	modcomponent = new sge::ModelComponent(modentity);
	modentity->setComponent(modcomponent);

	modtransform = new sge::TransformComponent(modentity);
	modentity->setComponent(modtransform);

	modcomponent->setModelResource(&modelHandle);
	modcomponent->setRenderingSystem(modelSystem);

	

	viewport = { 0, 0, 1280, 720 };

	modcomponent->setPipeline(pipeline);

	modelHandle.getResource<sge::ModelResource>()->createBuffers(sge::Spade::getInstance().getRenderer()->getDevice());

	sge::Spade::getInstance().getRenderer()->getDevice()->bindViewport(&viewport);

	// Bullet test
	broadphase = new btDbvtBroadphase();

	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));


	groundShape = new btBoxShape(btVector3(btScalar(51.), btScalar(1.), btScalar(51.)));
	topShape = new btBoxShape(btVector3(btScalar(51.), btScalar(1.), btScalar(51.)));
	
	wall1Shape = new btBoxShape(btVector3(btScalar(1.), btScalar(51.), btScalar(51.)));
	wall2Shape = new btBoxShape(btVector3(btScalar(51.), btScalar(51.), btScalar(1.)));
	wall3Shape = new btBoxShape(btVector3(btScalar(1.), btScalar(51.), btScalar(51.)));
	wall4Shape = new btBoxShape(btVector3(btScalar(51.), btScalar(51.), btScalar(1.)));
	
	fallShape = new btBoxShape(btVector3(1, 1, 1));

	// Floor
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(0, groundMotionState, groundShape, btVector4(0, 0, 1,1));
	groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);

	// Top
	btDefaultMotionState* topMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 25, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		topRigidBodyCI(0, topMotionState, topShape, btVector4(0, 0, 1, 1));
	topRigidBody = new btRigidBody(topRigidBodyCI);
	dynamicsWorld->addRigidBody(topRigidBody);

	// Wall 1
	btDefaultMotionState* wall1MotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(25, 25, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		wall1RigidBodyCI(0, wall1MotionState, wall1Shape, btVector4(0, 0, 1, 1));
	wall1RigidBody = new btRigidBody(wall1RigidBodyCI);
	dynamicsWorld->addRigidBody(wall1RigidBody);

	// Wall 2
	btDefaultMotionState* wall2MotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 25, -25)));
	btRigidBody::btRigidBodyConstructionInfo
		wall2RigidBodyCI(0, wall2MotionState, wall2Shape, btVector4(0, 0, 1, 1));
	wall2RigidBody = new btRigidBody(wall2RigidBodyCI);
	dynamicsWorld->addRigidBody(wall2RigidBody);

	// Wall 3
	btDefaultMotionState* wall3MotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-25, 25, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		wall3RigidBodyCI(0, wall3MotionState, wall3Shape, btVector4(0, 0, 1, 1));
	wall3RigidBody = new btRigidBody(wall3RigidBodyCI);
	dynamicsWorld->addRigidBody(wall3RigidBody);

	// Wall 4
	btDefaultMotionState* wall4MotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 25, 25)));
	btRigidBody::btRigidBodyConstructionInfo
		wall4RigidBodyCI(0, wall4MotionState, wall4Shape, btVector4(0, 0, 1, 1));
	wall4RigidBody = new btRigidBody(wall4RigidBodyCI);
	dynamicsWorld->addRigidBody(wall4RigidBody);

	// falling object
	btDefaultMotionState* fallMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 23, 0)));
	btScalar mass = 1;
	btVector3 fallInertia(0, 0, 0);
	fallShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
	fallRigidBodyCI.m_restitution = 1.0f;
	fallRigidBodyCI.m_friction = 0.5f;
	fallRigidBody = new btRigidBody(fallRigidBodyCI);
	fallRigidBody->setActivationState(DISABLE_DEACTIVATION);
	dynamicsWorld->addRigidBody(fallRigidBody);

	// falling object 2
	btDefaultMotionState* fallMotionState2 =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(5, 23, 0)));
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI2(mass, fallMotionState2, fallShape, fallInertia);
	fallRigidBodyCI2.m_restitution = 0.4f;
	fallRigidBodyCI2.m_friction = 0.8f;
	fallRigidBody2 = new btRigidBody(fallRigidBodyCI2);
	fallRigidBody2->setActivationState(DISABLE_DEACTIVATION);
	dynamicsWorld->addRigidBody(fallRigidBody2);

	camentity = EManager->createEntity();

	camcomponent = new sge::CameraComponent(camentity);
	camentity->setComponent(camcomponent);

	camtransform = new sge::TransformComponent(camentity);
	camentity->setComponent(camtransform);

	cameraPos = glm::vec3(5.0f, 10.0f, 50.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	camentity->getComponent<sge::CameraComponent>()->setupCamera(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f, false, cameraPos, cameraFront, cameraUp);

	uniformData2.M = glm::translate(glm::mat4(), glm::vec3(0.0f, 50.0f, 0.0f));
	uniformData2.PV = camentity->getComponent<sge::CameraComponent>()->getVp();
}

BulletTestScene::~BulletTestScene()
{
	dynamicsWorld->removeRigidBody(fallRigidBody);
	delete fallRigidBody->getMotionState();
	delete fallRigidBody;

	dynamicsWorld->removeRigidBody(fallRigidBody2);
	delete fallRigidBody2->getMotionState();
	delete fallRigidBody2;

	dynamicsWorld->removeRigidBody(groundRigidBody);
	delete groundRigidBody->getMotionState();
	delete groundRigidBody;

	dynamicsWorld->removeRigidBody(topRigidBody);
	delete topRigidBody->getMotionState();
	delete topRigidBody;

	dynamicsWorld->removeRigidBody(wall1RigidBody);
	delete wall1RigidBody->getMotionState();
	delete wall1RigidBody;
	dynamicsWorld->removeRigidBody(wall2RigidBody);
	delete wall2RigidBody->getMotionState();
	delete wall2RigidBody;
	dynamicsWorld->removeRigidBody(wall3RigidBody);
	delete wall3RigidBody->getMotionState();
	delete wall3RigidBody;
	dynamicsWorld->removeRigidBody(wall4RigidBody);
	delete wall4RigidBody->getMotionState();
	delete wall4RigidBody;

	delete fallShape;

	delete groundShape;
	delete topShape;

	delete wall1Shape;
	delete wall2Shape;
	delete wall3Shape;
	delete wall4Shape;


	delete dynamicsWorld;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;

	sge::Spade::getInstance().getRenderer()->getDevice()->debindPipeline(pipeline);


	sge::Spade::getInstance().getRenderer()->getDevice()->deleteShader(vertexShader);
	sge::Spade::getInstance().getRenderer()->getDevice()->deleteShader(pixelShader);
	
	sge::Spade::getInstance().getRenderer()->getDevice()->deletePipeline(pipeline);

	sge::Spade::getInstance().getResourceManager()->release(modelHandle);
}

void BulletTestScene::update(float step)
{
	dynamicsWorld->stepSimulation(step, 10);

	if (sge::Spade::getInstance().keyboardInput->keyIsPressed(sge::KEYBOARD_SPACE))
	{
		int randomx = sge::random(10, 500);
		int randomy = sge::random(10, 500);
		int randomz = sge::random(10, 500);
		//fallRigidBody->applyCentralImpulse(btVector3(0, 10, 0));
		fallRigidBody->applyTorque(btVector3(randomx, randomy, randomz));
		
	}

	if (sge::Spade::getInstance().keyboardInput->keyIsPressed(sge::KEYBOARD_P))
	{
		int randomx = sge::random(-10, 10);
		int randomy = sge::random(-10, 10);
		int randomz = sge::random(-10, 10);
		dynamicsWorld->setGravity(btVector3(randomx, randomy, randomz));
		
	}

	// Physics test with second object
	if (sge::Spade::getInstance().keyboardInput->keyIsPressed(sge::KEYBOARD_A))
	{
		fallRigidBody2->applyCentralImpulse(btVector3(-2, 0, 0));
		
		
	}

	if (sge::Spade::getInstance().keyboardInput->keyIsPressed(sge::KEYBOARD_D))
	{
		fallRigidBody2->applyCentralImpulse(btVector3( 2, 0, 0));

		
	}
	if (sge::Spade::getInstance().keyboardInput->keyIsPressed(sge::KEYBOARD_W))
	{
		fallRigidBody2->applyCentralImpulse(btVector3(0, -0, -2));

		
	}

	if (sge::Spade::getInstance().keyboardInput->keyIsPressed(sge::KEYBOARD_S))
	{
		fallRigidBody2->applyCentralImpulse(btVector3(0, 0, 2));

		
	}

	btTransform trans;
	fallRigidBody->getMotionState()->getWorldTransform(trans);

	//std::cout << "Box height: " << trans.getOrigin().getY() << std::endl;
	
	sge::math::mat4 plaa = sge::math::translate(sge::math::mat4(), sge::math::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
	uniformData2.M = sge::math::rotate(plaa, trans.getRotation().getAngle(), sge::math::vec3(trans.getRotation().getAxis().getX(), trans.getRotation().getAxis().getY(), trans.getRotation().getAxis().getZ()));

	if (sge::Spade::getInstance().keyboardInput->keyIsPressed(sge::KEYBOARD_ESCAPE))
	{
		sge::Spade::getInstance().stop();
	}

	if (sge::Spade::getInstance().keyboardInput->keyIsPressed(sge::KEYBOARD_F1))
	{
		camentity->getComponent<sge::CameraComponent>()->enableMouse();
	}
	if (sge::Spade::getInstance().keyboardInput->keyIsPressed(sge::KEYBOARD_F2))
	{
		camentity->getComponent<sge::CameraComponent>()->disableMouse();
	}

	camentity->getComponent<sge::CameraComponent>()->update();
	modelSystem->setVP(camentity->getComponent<sge::CameraComponent>()->getVp());
}
void BulletTestScene::draw()
{
	sge::Spade::getInstance().getRenderer()->getDevice()->clear(0.5f, 0.0f, 0.5f, 1.0f);

	modentity->getComponent<sge::ModelComponent>()->render(sge::Spade::getInstance().getRenderer()->getDevice());

	sge::Spade::getInstance().getRenderer()->getDevice()->swap();
}

void BulletTestScene::interpolate(float alpha)
{
}
