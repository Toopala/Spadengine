#include "GameScene.h"
#include "Spade/Spade.h"
#include "Game/Entity.h"
#include "Renderer/Texture.h"
#include "Renderer/RenderTarget.h"

/*
TODO

Input komponenteille tai kamerakomponentin fiksaus siten et inputtia ei saa spaden singletonin kautta
Tekstuuriresurssille tekstuurien generointi GPU:n muistiin
VP:n anto renderöintisysteemeille typerää (pointteri matriisiin??) Ehkä vois passata kameran
Kameran anto renderöintisysteemeille ja se laskee ne etäisyydet sitte sen kans
Projektiomatriisin nearin ja farin määrittely niin et toimii samaten DX ja GL
Korjaa DX 11:n depth-tarkistus
Kamerakomponentti saamaan positio transformilta ja muutenkin refaktoroida

*/

GameScene::GameScene(sge::Spade* engine) :
    engine(engine),
    renderer(engine->getRenderer()),
    textureResource(sge::ResourceManager::getMgr().load<sge::TextureResource>("../Assets/spade.png")),
    texture(engine->getRenderer()->getDevice()->createTexture(
        textureResource.getResource<sge::TextureResource>()->getSize().x,
        textureResource.getResource<sge::TextureResource>()->getSize().y,
        textureResource.getResource<sge::TextureResource>()->getData())),
    targetTexture(nullptr),
    renderTarget(nullptr)
{
    // TODO initialization should be easier.
    // TODO downloading resources and creating textures is messy.

    cameras.push_back(createCamera(0,   0,      640, 360));
    cameras.push_back(createCamera(640, 0,      640, 360));
    cameras.push_back(createCamera(640, 360,    640, 360));
    cameras.push_back(createCamera(0,   360,    640, 360));

    entities.push_back(createEntity(256.0f, 256.0f, 256.0f, 256.0f, 2.0f, 1.0f, 0.0f, 0.0f, 1.0f ));
    entities.back()->setTag("BEHNID");

    entities.push_back(createEntity(192.0f, 256.0f, 256.0f, 256.0f, 2.1f, 0.5f, 0.5f, 0.5f, 0.5f ));
    entities.back()->setTag("FRONT");

    targetTexture = renderer->getDevice()->createTexture(1280, 720);
    renderTarget = renderer->getDevice()->createRenderTarget(targetTexture);
}

GameScene::~GameScene()
{
    // TODO this should probably be not deleted here, but in the texture resource.
    renderer->getDevice()->deleteTexture(texture);
    renderer->getDevice()->deleteTexture(targetTexture);
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
    // TODO should we move begin and end to somewhere else?
    renderer->begin();
    renderer->setRenderTargets(1, renderTarget);

    renderer->setCameras(cameras.size(), *cameras.data());
    renderer->renderSprites(entities.size(), *entities.data());

    renderer->end();
    renderer->present();
    renderer->clear();
}

sge::Entity* GameScene::createEntity(float x, float y, float width, float height, float depth, float r, float g, float b, float a)
{
    // TODO this should be easier.
    sge::Entity* player = entityManager.createEntity();

    auto transform = transformFactory.create(player);
    auto sprite = spriteFactory.create(player);

    transform->setPosition({ x, y, depth });
    transform->setScale({ width, height, 1.0f });
    transform->setRotationVector({ 0.0f, 0.0f, 1.0f });
    transform->setAngle(45.0f);

    sprite->setTexture(texture);
    sprite->setColor({ r, g, b, a });

    return player;
}

sge::Entity* GameScene::createCamera(int x, int y, unsigned int width, unsigned int height)
{
    // TODO cameracomponent doesn't use transform component and directly
    // requests input from (old design) spade singleton. These should be fixed.
    // Input system needs more planning to do.
    sge::Entity* camera = entityManager.createEntity();

    auto transform = transformFactory.create(camera);
    auto cameracomponent = cameraFactory.create(camera);

    transform->setPosition({ 0.0f, 0.0f, 10.0f });
    transform->setFront({ 0.0f, 0.0f, -1.0f });
    transform->setUp({ 0.0f, 1.0f, 0.0f });

    cameracomponent->setOrtho(0.0f, (float)width, (float)height, 0.0f, 0.1f, 1000.0f);
    cameracomponent->setViewport(x, y, width, height);

    return camera;
}