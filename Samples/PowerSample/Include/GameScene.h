#include "Game/Scene.h"
#include "Game/EntityManager.h"
#include "Game/SpriteRenderingSystem.h"
#include "Game/ComponentFactory.h"
#include "Game/TransformComponent.h"
#include "Game/SpriteComponent.h"
#include "Game/CameraComponent.h"
#include "Resources/ResourceManager.h"
#include "Resources/TextureResource.h"
#include "Core/Math.h"
#include "Renderer/Viewport.h"


// Forward declares
namespace sge
{
    class Spade;
    class Entity;
    struct Texture;
}

class GameScene : public sge::Scene
{
public:
    GameScene(sge::Spade* engine);
    ~GameScene();

    void update(float step);
    void interpolate(float alpha);
    void draw();

    // TODO just a temporary way to create entities.
    // OR a proper way?
    sge::Entity* createEntity(float x, float y, float width, float height, float depth, float r, float g, float b, float a);
    sge::Entity* createCamera(int x, int y, unsigned int width, unsigned int height);

private:
    sge::Spade* engine;
    std::vector<sge::Entity*> entities;
    std::vector<sge::Entity*> cameras;
    sge::Handle<sge::TextureResource> textureResource;
    sge::Texture* texture;
    sge::math::mat4 VP;

    sge::EntityManager entityManager;

    // TODO do we really need system manager? ._.
    sge::SpriteRenderingSystem spriteRenderingSystem;
    
    // Component factories
    // TODO maybe we should use one factory for all component types? Using it could be nicer.
    sge::ComponentFactory<sge::TransformComponent> transformFactory;
    sge::ComponentFactory<sge::SpriteComponent> spriteFactory;
    sge::ComponentFactory<sge::CameraComponent> cameraFactory;
};