#ifdef DIRECTX11

#pragma once

#include <d3d11.h>

#include "Renderer/VertexLayout.h"

namespace sge
{
	struct DX11VertexLayout
	{
		VertexLayout header;

		ID3D11InputLayout* inputLayout;
	};
}

#endif