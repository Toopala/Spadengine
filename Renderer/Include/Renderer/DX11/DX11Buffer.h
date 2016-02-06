#ifdef DIRECTX11

#pragma once

#include <d3d11.h>

#include "Renderer/Buffer.h"

namespace sge
{
	struct DX11Buffer
	{
		Buffer header;

		ID3D11Buffer* buffer;
	};
}

#endif