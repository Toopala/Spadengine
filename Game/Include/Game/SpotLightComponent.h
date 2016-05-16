#pragma once

#include "Game/LightComponent.h"

namespace sge
{
    class SpotLightComponent : public LightComponent
    {
    public:
        SpotLightComponent(Entity* ent);
        ~SpotLightComponent();

        void update();
    private:
    };
}