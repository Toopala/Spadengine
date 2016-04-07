#pragma once

#include "Core/Types.h"

namespace sge
{
	union RenderCommand
	{
		struct Fields
		{
            uint64 data : 31;
            uint64 depth : 32;
            uint64 translucent : 1;
        } fields;

		uint64 bits;
	};

    // TODO translucent draws should be sorted by depth and from back to front
    // while opaque draws should be sorted by used shaders, textures etc. and 
    // from front to back. 
}