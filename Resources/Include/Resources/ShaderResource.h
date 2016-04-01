#pragma once

#include "Resources/Resource.h"
#include <vector>

namespace sge
{
    class ShaderResource : public Resource
    {
    public:
        ShaderResource(const std::string& resourcePath);
        ~ShaderResource();

        const std::vector<char>& loadShader();

    private:

        std::vector<char> data;

    };
}
