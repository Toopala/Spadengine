#include "GameScene.h"
#include "Spade/Spade.h"
#include "Game/Entity.h"
#include "Renderer/Texture.h"

GameScene::GameScene(sge::Spade* engine) :
    engine(engine),
    player(nullptr),
    textureResource(sge::ResourceManager::getMgr().load<sge::TextureResource>("../Assets/spade.png")),
    texture(engine->getRenderer()->getDevice()->createTexture(
        textureResource.getResource<sge::TextureResource>()->getSize().x,
        textureResource.getResource<sge::TextureResource>()->getSize().y,
        textureResource.getResource<sge::TextureResource>()->getData())),
    spriteRenderingSystem(engine->getRenderer()),
    VP(sge::math::ortho(0.0f, 1280.0f, 720.0f, 0.0f)),
    viewport({ 0, 0, 1280, 720 })
{
    // TODO initialization should be easier.
    // TODO get VP from camera.
    // TODO downloading resources and creating textures is messy.
    // TODO binding viewport? Getting device from renderer from spade is a long way.
    createPlayer();
    spriteRenderingSystem.setVP(VP);
    engine->getRenderer()->getDevice()->bindViewport(&viewport);
}

GameScene::~GameScene()
{
    engine->getRenderer()->getDevice()->deleteTexture(texture);
}

void GameScene::update(float step)
{
    // Simple rotation test.
    static float alpha = 0.0f;

    alpha += 0.01f;

    player->getComponent<sge::TransformComponent>()->setAngle(alpha);
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

    transform->setPosition({ 256.0f, 256, 0.0f });
    transform->setScale({ textureResource.getResource<sge::TextureResource>()->getSize().x / 2.0f, textureResource.getResource<sge::TextureResource>()->getSize().y / 2.0f, 1.0f });
    transform->setRotationVector({ 0.5f, 0.7f, 1.0f });
    transform->setAngle(45.0f);

    sprite->setTexture(texture);
    sprite->setColor({ 0.0f, 0.5f, 0.5f, 1.0f });

    spriteRenderingSystem.addComponent(sprite);
}