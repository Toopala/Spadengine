#pragma once

#include "Game/LightComponent.h"

namespace sge
{
    struct DirLight
    {
        sge::math::vec4 direction;
        sge::math::vec4 ambient;
        sge::math::vec4 diffuse;
        sge::math::vec4 specular;
    };

    class DirLightComponent : public LightComponent
    {
    public:
        DirLightComponent(Entity* ent);
        ~DirLightComponent();

        void update();

        const DirLight& getLightData() { return lightData; }
        void setLightData(const DirLight& data) { lightData = data; }
    private:
        DirLight lightData;
    };
}