#include "GameScene.h"
#include "Spade/Spade.h"
#include "Game/Entity.h"
#include "Renderer/Texture.h"

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
    player(nullptr),
    camera(nullptr),
    textureResource(sge::ResourceManager::getMgr().load<sge::TextureResource>("../Assets/spade.png")),
    texture(engine->getRenderer()->getDevice()->createTexture(
        textureResource.getResource<sge::TextureResource>()->getSize().x,
        textureResource.getResource<sge::TextureResource>()->getSize().y,
        textureResource.getResource<sge::TextureResource>()->getData())),
    spriteRenderingSystem(engine->getRenderer())
{
    // TODO initialization should be easier.
    // TODO downloading resources and creating textures is messy.

    createPlayer();
    createCamera();
}

GameScene::~GameScene()
{
    // TODO this should probably be not deleted here, but in the texture resource.
    engine->getRenderer()->getDevice()->deleteTexture(texture);
}

void GameScene::update(float step)
{
    // TODO sprite should be moving 32 pixels per second!
    sge::math::vec3 speed(0.0f, 64.0f * step, 0.0f);
    float alpha = 5.0f * step;

    // TODO how about moving this input checking to input component or something?
    if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_UP))
    {
        player->getComponent<sge::TransformComponent>()->addPosition(-speed);
    }

    if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_DOWN))
    {
        player->getComponent<sge::TransformComponent>()->addPosition(speed);
    }

    if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_LEFT))
    {
        player->getComponent<sge::TransformComponent>()->addAngle(alpha);
    }

    if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_RIGHT))
    {
        player->getComponent<sge::TransformComponent>()->addAngle(-alpha);
    }

    if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_ESCAPE))
    {
        // TODO weird but works?
        engine->stop();
    }

    camera->getComponent<sge::TransformComponent>()->addPosition({ 32.0f * step, 0.0f, 0.0f });

    // TODO update camera where? What if we have multiple cameras? Do we need a system for them?
    camera->getComponent<sge::CameraComponent>()->update();

    // TODO binding viewport? Getting device from renderer from spade is a long way.
    // Also looks awful.
    engine->getRenderer()->getDevice()->bindViewport(camera->getComponent<sge::CameraComponent>()->getViewport());

    // TODO can we simplify this? Do we need to set VP every frame manually?
    spriteRenderingSystem.setVP(camera->getComponent<sge::CameraComponent>()->getViewProj());
}

void GameScene::interpolate(float alpha)
{

}

void GameScene::draw()
{
    // TODO should we move begin and end to somewhere else?
    engine->getRenderer()->begin();
    spriteRenderingSystem.update();
    engine->getRenderer()->end();
}

void GameScene::createPlayer()
{
    // TODO this should be easier.
    player = entityManager.createEntity();

    auto transform = transformFactory.create(player);
    auto sprite = spriteFactory.create(player);

    transform->setPosition({ 256.0f, 256.0f, -256.0f });
    transform->setScale({ textureResource.getResource<sge::TextureResource>()->getSize().x / 2.0f, textureResource.getResource<sge::TextureResource>()->getSize().y / 2.0f, 1.0f });
    transform->setRotationVector({ 0.5f, 0.7f, 1.0f });
    transform->setAngle(45.0f);

    sprite->setTexture(texture);
    sprite->setColor({ 0.0f, 0.5f, 0.5f, 1.0f });

    // TODO should spriterenderingsystem give you the components? Could make things nicer.
    spriteRenderingSystem.addComponent(sprite);
}

void GameScene::createCamera()
{
    // TODO cameracomponent doesn't use transform component and directly
    // requests input from (old design) spade singleton. These should be fixed.
    // Input system needs more planning to do.
    camera = entityManager.createEntity();

    auto transform = transformFactory.create(camera);
    auto cameracomponent = cameraFactory.create(camera);

    transform->setPosition({ 0.0f, 0.0f, 0.0f });
    transform->setFront({ 0.0f, 0.0f, -1.0f });
    transform->setUp({ 0.0f, 1.0f, 0.0f });

    cameracomponent->setOrtho(0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 1000.0f);
    cameracomponent->setViewport(0, 0, 1280, 720);
}