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
    void createPlayer();
    void createCamera();

private:
    sge::Spade* engine;
    sge::Entity* player;
    sge::Entity* camera;
    sge::Handle<sge::TextureResource> textureResource;
    sge::Texture* texture;
    sge::math::mat4 VP;
    sge::Viewport viewport;

    sge::EntityManager entityManager;

    // TODO do we really need system manager? ._.
    sge::SpriteRenderingSystem spriteRenderingSystem;
    
    // Component factories
    // TODO maybe we should use one factory for all component types? Using it could be nicer.
    sge::ComponentFactory<sge::TransformComponent> transformFactory;
    sge::ComponentFactory<sge::SpriteComponent> spriteFactory;
    sge::ComponentFactory<sge::CameraComponent> cameraFactory;
};