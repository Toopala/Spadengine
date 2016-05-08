#pragma once

#include "Game/Component.h"

namespace sge
{
    class Entity;

    class LightComponent : public Component
    {
    public:
        LightComponent(Entity* entity);
        ~LightComponent();

        void update(float step);
    private:
    };
}