#ifdef DIRECTX11

#pragma once

#include <d3d11.h>

#include "Renderer/Texture.h"

namespace sge
{
	struct DX11Texture
	{
		Texture header;

		ID3D11Texture2D* texture;
		ID3D11ShaderResourceView* textureView;
	};
}

#endif