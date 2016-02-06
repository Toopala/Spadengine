#ifdef DIRECTX11

#pragma once

#include <d3d11.h>

#include "Renderer/DX11/DX11Shader.h"
#include "Renderer/Pipeline.h"

namespace sge
{
	struct DX11Pipeline
	{
		Pipeline header;

		DX11Shader* vertexShader;
		DX11Shader* pixelShader;
		ID3D11InputLayout* inputLayout;
	};
}

#endif