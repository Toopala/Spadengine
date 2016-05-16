#include "Game/DirLightComponent.h"

namespace sge
{
    DirLightComponent::DirLightComponent(Entity* entity) :
        LightComponent(entity)
    {
        lightData.direction = math::vec4(-1.0, -1.0, -1.0, 1.0);
        lightData.ambient = math::vec4(0.05, 0.05, 0.05, 1.0);
        lightData.diffuse = math::vec4(0.8, 0.8, 0.8, 1.0);
        lightData.specular = math::vec4(0.5, 0.5, 0.5, 1.0);
    }

    DirLightComponent::~DirLightComponent()
    {

    }

    void DirLightComponent::update()
    {

    }
}