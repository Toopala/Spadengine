#include "Resources/ShaderResource.h"
#include "Core/Assert.h"
#include <fstream>
#include <sstream>

namespace sge
{
    ShaderResource::ShaderResource(const std::string& resourcePath) : sge::Resource(resourcePath)
    {

    }

    ShaderResource::~ShaderResource()
    {

    }

    const std::vector<char>& ShaderResource::loadShader()
    {

        std::ifstream file;

        if (resourcePath.find(".glsl") != std::string::npos)
        {
            file.open(resourcePath, std::ios::in);

            if (file.is_open())
            {
                std::cout << "SUCCESS: Opened shader " << resourcePath << std::endl;

                std::stringstream stream;
                std::string str;

                stream << file.rdbuf();

                str = stream.str();

                std::copy(str.begin(), str.end(), std::back_inserter(data));

                data.push_back('\0');

                file.close();
            }
        }

        else if (resourcePath.find(".cso") != std::string::npos)
        {
            file.open(resourcePath, std::ios::in | std::ios::ate | std::ios::binary);

            if (file.is_open())
            {
                std::cout << "SUCCESS: Opened shader " << resourcePath << std::endl;
                data.resize(static_cast<size_t>(file.tellg()));

                file.seekg(0, std::ios::beg);
                file.read(data.data(), data.size());
                file.close();
            }
        }

        else
        {
            std::cout << "ERROR: Could not open shader: " << resourcePath << std::endl;
            SGE_ASSERT(false);
        }

        return data;
    }
}