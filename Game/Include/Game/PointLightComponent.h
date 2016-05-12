#pragma once

#include "Game/LightComponent.h"

namespace sge
{
    struct PointLight
    {
        sge::math::vec4 position;
        sge::math::vec4 ambient;
        sge::math::vec4 diffuse;
        sge::math::vec4 specular;

        float constant;
        float mylinear;
        float quadratic;
        float pad;
    };

    class PointLightComponent : public LightComponent
    {
    public:
        PointLightComponent(Entity* ent);
        ~PointLightComponent();

        void update();

        const PointLight& getLightData() { return lightData; }
        void setLightData(const PointLight& data) { lightData = data; }
    private:
        PointLight lightData;
    };
}