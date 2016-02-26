#pragma once

#include "Core/Types.h"

namespace sge
{
	union RenderCommand
	{
		struct
		{
			uint64 data : 53;
			uint64 command : 1;
			uint64 translucent : 2;
			uint64 viewportLayer : 3;
			uint64 viewport : 3;
			uint64 fullscreenLayer : 2;
		};

		uint64 bits;
	};
}