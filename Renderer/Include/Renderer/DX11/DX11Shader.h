#ifdef DIRECTX11

#pragma once

#include <d3d11.h>
#include "Renderer/Shader.h"

namespace sge
{
	struct DX11Shader
	{
		Shader header;

		union
		{
			ID3D11VertexShader* vertexShader;
			ID3D11PixelShader* pixelShader;
		};

		// TODO super hax
		const char* source;
		size_t size;
	};
}

#endif