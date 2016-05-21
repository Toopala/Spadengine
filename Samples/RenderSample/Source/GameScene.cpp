#include "GameScene.h"
#include "Spade/Spade.h"
#include "Game/Entity.h"
#include "Renderer/Texture.h"
#include "Renderer/RenderTarget.h"

GameScene::GameScene(sge::Spade* engine) :
    engine(engine),
    renderer(engine->getRenderer()),
    textureResource(sge::ResourceManager::getMgr().load<sge::TextureResource>("../Assets/spade.png")),
    fontResource(sge::ResourceManager::getMgr().load<sge::FontResource>("../Assets/verdana.ttf")),
    texture(engine->getRenderer()->getDevice()->createTexture(
        textureResource.getResource<sge::TextureResource>()->getSize().x,
        textureResource.getResource<sge::TextureResource>()->getSize().y,
        textureResource.getResource<sge::TextureResource>()->getData())),
    renderTarget(nullptr)
{
    // TODO initialization should be easier.
    // TODO downloading resources and creating textures is messy.

    const int border = 16;
    const int screenAreaWidth = 1280 - border * 2;
    const int screenAreaHeight = 720 - border * 2;
    const int camWidth = screenAreaWidth / 2;
    const int camHeight = screenAreaHeight / 2;

    cameras.push_back(createCamera(0, 0,                camWidth, camHeight));
    cameras.push_back(createCamera(camWidth, 0,         camWidth, camHeight));
    cameras.push_back(createCamera(camWidth, camHeight, camWidth, camHeight));
    cameras.push_back(createCamera(0, camHeight,        camWidth, camHeight));

    fullscreenCamera = createCamera(border, border, screenAreaWidth, screenAreaHeight);

    entities.push_back(createEntity(texture, 256.0f, 256.0f, 96.0f, 64.0f, 2.0f, 1.0f, 0.0f, 0.0f, 1.0f, 25.0f));
    entities.back()->setTag("BEHNID");

    entities.push_back(createEntity(texture, 192.0f, 256.0f, 64.0f, 64.0f, 2.1f, 0.5f, 0.5f, 0.5f, 1.0f, 50.0f));
    entities.back()->setTag("FRONT");

	textEntities.push_back(createText(100, 100, "Spadengine"));

    guiText = createText(256.0f, 256.0f, "YOLO :D:::D");

    renderTarget = renderer->getDevice()->createRenderTarget(4, 1280, 720);

    targetEntity = createEntity(renderTarget->textures[0], 1280.0f / 2, 720.0f / 2, 1280.0f / 2, 720.0f / 2, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
}

GameScene::~GameScene()
{
    // TODO this should probably be not deleted here, but in the texture resource.
    renderer->getDevice()->deleteTexture(texture);

    renderer->getDevice()->deleteRenderTarget(renderTarget);
}

void GameScene::update(float step)
{
    // TODO sprite should be moving 32 pixels per second!
    sge::math::vec3 speed(0.0f, 64.0f * step, 0.0f);
    float alpha = 5.0f * step;

    // TODO how about moving this input checking to input component or something?
    if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_UP))
    {
        entities.front()->getComponent<sge::TransformComponent>()->addPosition(-speed);
    }

    if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_DOWN))
    {
        entities.front()->getComponent<sge::TransformComponent>()->addPosition(speed);
    }

    if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_LEFT))
    {
        entities.front()->getComponent<sge::TransformComponent>()->addAngle(alpha);
    }

    if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_RIGHT))
    {
        entities.front()->getComponent<sge::TransformComponent>()->addAngle(-alpha);
    }

    if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_ESCAPE))
    {
        // TODO weird but works?
        engine->stop();
    }

    cameras[2]->getComponent<sge::TransformComponent>()->addPosition({ 32.0f * step, 0.0f, 0.0f });

    // TODO update camera where? What if we have multiple cameras? Do we need a system for them?
    for (auto camera : cameras)
    {
        camera->getComponent<sge::CameraComponent>()->update();
    }

    fullscreenCamera->getComponent<sge::CameraComponent>()->update();

    // TODO binding viewport? Getting device from renderer from spade is a long way.
    // Also looks awful. So awful.
    //engine->getRenderer()->getDevice()->bindViewport(camera->getComponent<sge::CameraComponent>()->getViewport());

    // TODO can we simplify this? Do we need to set VP every frame manually?
    //spriteRenderSystem.setVP(cameras.back()->getComponent<sge::CameraComponent>()->getViewProj());
}

void GameScene::interpolate(float alpha)
{

}

void GameScene::draw()
{
    // Note that we need to set render targets and cameras before we begin.
    // First pass
    renderer->setRenderTarget(renderTarget);
    renderer->addCameras(cameras.size(), cameras.data());
    renderer->setClearColor(0.5f, 0.0f, 0.5, 1.0f);
    renderer->clear(sge::COLOR);

    renderer->begin();
    renderer->renderSprites(entities.size(), entities.data());
	renderer->renderTexts(textEntities.size(), textEntities.data());
    renderer->end();

    renderer->render();

    // Second pass
    renderer->setClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    renderer->clear();

    renderer->addCameras(1, &fullscreenCamera);

    renderer->begin();
    renderer->renderSprites(1, &targetEntity);
    renderer->end();

    renderer->render();

    // Present to screen
    renderer->present();
    renderer->clear();
}

sge::Entity* GameScene::createEntity(sge::Texture* texture, float x, float y, float width, float height, float depth, float r, float g, float b, float a, float angle)
{
    // TODO this should be easier.
    sge::Entity* entity = entityManager.createEntity();

    auto transform = transformFactory.create(entity);
    auto sprite = spriteFactory.create(entity);

    transform->setPosition({ x, y, depth });
    transform->setScale({ width, height, 1.0f });
    transform->setRotationVector({ 0.0f, 0.0f, 1.0f });
    transform->setAngle(angle);

    sprite->setTexture(texture);
    sprite->setColor({ r, g, b, a });

    return entity;
}

sge::Entity* GameScene::createCamera(int x, int y, unsigned int width, unsigned int height)
{
    sge::Entity* entity = entityManager.createEntity();

    auto transform = transformFactory.create(entity);
    auto cameracomponent = cameraFactory.create(entity);

    transform->setPosition({ 0.0f, 0.0f, 10.0f });
    transform->setFront({ 0.0f, 0.0f, -1.0f });
    transform->setUp({ 0.0f, 1.0f, 0.0f });

    cameracomponent->setOrtho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 1000.0f);
    cameracomponent->setViewport(x, y, width, height);

    return entity;
}

sge::Entity* GameScene::createText(float x, float y, const std::string& text)
{
    sge::Entity* entity = entityManager.createEntity();

    auto transform = transformFactory.create(entity);
    auto textcomponent = textFactory.create(entity);
    
    transform->setPosition({ x, y, 0.0f });
	transform->setRotationVector({ 0.0f, 0.0f, 1.0f });

    textcomponent->setColor({ 0.0f, 0.0f, 0.0f, 1.0f });
    textcomponent->setFont(fontResource.getResource<sge::FontResource>()->getFont());
    textcomponent->setText(text);

    return entity;
}