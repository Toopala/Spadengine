#include "GameScene.h"

#include "Game/Entity.h"
#include "Game/CameraComponent.h"
#include "Game/ModelComponent.h"
#include "Game/TransformComponent.h"
#include "Game/RenderSystem.h"

#include "Renderer/Pipeline.h"
#include "Renderer/VertexLayout.h"

#include "Resources/ShaderResource.h"

#include "Spade/Spade.h"

/*
TODO enko

Tekstuuriresurssille tekstuurien generointi GPU:n muistiin
SpotLightComponent do!
Instanced rendering do!
Deferred rendering do!
Render targeteille depth-puskuri!
Optimoi bulletscenea (nykii ihan vitusti)
    - Esim valojen dataa ei tarvii viedä joka objektille erikseen
    - Sorttaus takasin kuntoon jos tarviijaksaahaluaa
Tee se demo ja siihen kaikkea hauskaa (blur, hdr voe pojat efektejä)
DX11-shaderit!
*/

GameScene::GameScene(sge::Spade* engine) :
    engine(engine),
    renderer(engine->getRenderer()),
    device(renderer->getDevice())
{
    initPipelines();
    initResources();

    cameraEntity = createCamera(0, 0, 1280, 720);
    earthEntity = createEarth();
}

GameScene::~GameScene()
{
    sge::ResourceManager::getMgr().release(earthResource);

    device->deleteShader(vertexShader);
    device->deleteShader(pixelShader);

    device->deletePipeline(pipeline);
}

void GameScene::update(float step)
{
    if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_ESCAPE))
    {
        engine->stop();
    }

    cameraEntity->getComponent<sge::CameraComponent>()->update();
}

void GameScene::draw()
{
    renderer->addCameras(1, &cameraEntity);

    renderer->begin();
    renderer->renderModels(1, &earthEntity);
    renderer->end();

    renderer->render();
    renderer->present();
    renderer->clear();
}

sge::Entity* GameScene::createEarth()
{
    sge::Entity* entity = entityManager.createEntity();

    auto transform = transformFactory.create(entity);
    auto model = modelFactory.create(entity);

    model->setPipeline(pipeline);
    model->setShininess(15.0f);
    model->setRenderer(renderer);
    model->setModelResource(&earthResource);

    return entity;
}

sge::Entity* GameScene::createCamera(int x, int y, unsigned int width, unsigned int height)
{
    sge::Entity* entity = entityManager.createEntity();

    auto transform = transformFactory.create(entity);
    auto cameracomponent = cameraFactory.create(entity);

    transform->setPosition({ 5.0f, 0.0f, 48.0f });
    transform->setFront({ 0.0f, 0.0f, -1.0f });
    transform->setUp({ 0.0f, 1.0f, 0.0f });

    cameracomponent->setPerspective(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    cameracomponent->setViewport(x, y, width, height);

    return entity;
}

void GameScene::initPipelines()
{
    sge::Handle<sge::ShaderResource> pixelShaderHandle;
    sge::Handle<sge::ShaderResource> vertexShaderHandle;

#ifdef DIRECTX11
    vertexShaderHandle = sge::ResourceManager::getMgr().load<sge::ShaderResource>("../Assets/Shaders/VertexShaderLights.cso");
    pixelShaderHandle = sge::ResourceManager::getMgr().load<sge::ShaderResource>("../Assets/Shaders/PixelShaderLights.cso");
#elif OPENGL4
    vertexShaderHandle = sge::ResourceManager::getMgr().load<sge::ShaderResource>("../Assets/Shaders/VertexShaderLights.glsl");
    pixelShaderHandle = sge::ResourceManager::getMgr().load<sge::ShaderResource>("../Assets/Shaders/PixelShaderLights.glsl");
#endif

    const std::vector<char>& vertexShaderData = vertexShaderHandle.getResource<sge::ShaderResource>()->loadShader();
    const std::vector<char>& pixelShaderData = pixelShaderHandle.getResource<sge::ShaderResource>()->loadShader();

    vertexShader = device->createShader(sge::ShaderType::VERTEX, vertexShaderData.data(), vertexShaderData.size());
    pixelShader = device->createShader(sge::ShaderType::PIXEL, pixelShaderData.data(), pixelShaderData.size());

    sge::VertexLayoutDescription vertexLayoutDescription = { 5,
    {
        { 0, 3, sge::VertexSemantic::POSITION },
        { 0, 3, sge::VertexSemantic::NORMAL },
        { 0, 3, sge::VertexSemantic::TANGENT },
        { 0, 3, sge::VertexSemantic::TANGENT },
        { 0, 2, sge::VertexSemantic::TEXCOORD }
    } };

    pipeline = device->createPipeline(&vertexLayoutDescription, vertexShader, pixelShader);

    device->bindPipeline(pipeline);
}

void GameScene::initResources()
{
    earthResource = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/Models/earth.obj");
    earthResource.getResource<sge::ModelResource>()->setDevice(device);
    earthResource.getResource<sge::ModelResource>()->createBuffers();
}

void GameScene::interpolate(float alpha)
{
}