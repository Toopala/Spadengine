#include "Game/PointLightComponent.h"
#include "Game/TransformComponent.h"

namespace sge
{
    PointLightComponent::PointLightComponent(Entity* entity) :
        LightComponent(entity)
    {
    }

    PointLightComponent::~PointLightComponent()
    {

    }

    void PointLightComponent::update()
    {
        lightData.position = math::vec4(transform->getPosition(), 1.0f);
    }
}