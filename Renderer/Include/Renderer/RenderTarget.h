#pragma once

#include "Renderer/Texture.h"

namespace sge
{
    struct RenderTarget
    {
        Texture** textures;
        size_t count;
    };
}