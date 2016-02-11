#ifdef DIRECTX11

#pragma once

#include <d3d11.h>

#include "Renderer/DX11/DX11Shader.h"
#include "Renderer/DX11/DX11VertexLayout.h"
#include "Renderer/Pipeline.h"

namespace sge
{
	struct DX11Pipeline
	{
		Pipeline header;

		DX11Shader* vertexShader;
		DX11Shader* pixelShader;
		DX11VertexLayout* vertexLayout;
		ID3D11SamplerState* samplerState;
		ID3D11RasterizerState* rasterizerState;
	};
}

#endif