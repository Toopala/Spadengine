#ifdef OPENGL4

#pragma once

#include "glad/glad.h"

#include "Renderer/RenderTarget.h"

namespace sge
{
    struct GL4RenderTarget
    {
        RenderTarget header;

        GLuint id;
        GLenum* buffers;
        size_t count;
    };
}

#endif