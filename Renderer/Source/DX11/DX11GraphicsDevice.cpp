#ifdef DIRECTX11

#include <Windows.h>
#include <d3d11.h>
#include <comdef.h>

#pragma comment (lib, "d3d11.lib")

#include <iostream>
#include "SDL2/SDL_syswm.h"

#include "Core/Assert.h"
#include "Renderer/DX11/DX11Buffer.h"
#include "Renderer/DX11/DX11Pipeline.h"
#include "Renderer/DX11/DX11RenderTarget.h"
#include "Renderer/DX11/DX11Shader.h"
#include "Renderer/DX11/DX11Texture.h"
#include "Renderer/DX11/DX11VertexLayout.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/VertexLayout.h"
#include "Renderer/Viewport.h"
#include "Renderer/Window.h"

namespace sge
{
	void checkError(HRESULT result)
	{
		if (result != S_OK)
		{
			_com_error err(result);
			LPCTSTR errMsg = err.ErrorMessage();

			std::cout << "DX ERROR: " << errMsg << std::endl;

			SGE_ASSERT(false);
		}
	}

	struct GraphicsDevice::Impl
	{
		Impl(Window& window) : 
			window(window),
			hdc(nullptr), 
			hwnd(nullptr),
			device(NULL),
			context(NULL),
			swapChain(NULL),
			debug(NULL),
			pipeline(nullptr),
			backBufferTexture(NULL),
			depthStencilBuffer(NULL),
			depthStencilView(NULL)
		{
			// Get windows handle from SDL.
			SDL_SysWMinfo info;

			SDL_VERSION(&info.version);

			if (SDL_GetWindowWMInfo(window.getSDLWindow(), &info))
			{
				switch (info.subsystem)
				{
				case SDL_SYSWM_WINDOWS:
				{
					hwnd = info.info.win.window;
					hdc = info.info.win.hdc;
				} break;
				default: break;
				}
			}
		}

		~Impl()
		{
		}

		Window& window;
		HDC hdc;
		HWND hwnd;
		ID3D11Device* device;
		ID3D11DeviceContext* context;
		IDXGISwapChain* swapChain;
		ID3D11Debug* debug;
		DX11Pipeline* pipeline;
		ID3D11Texture2D* backBufferTexture;
		ID3D11Texture2D* depthStencilBuffer;
		ID3D11DepthStencilView* depthStencilView;
        DX11RenderTarget* currentRenderTarget;
        DX11RenderTarget* defaultRenderTarget;
	};

	GraphicsDevice::GraphicsDevice(Window& window) :
		impl(new Impl(window))
	{
	}

	GraphicsDevice::~GraphicsDevice()
	{
		delete impl;
	}

	void GraphicsDevice::init()
	{
		// Create device and swap chain.
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_11_0;
		D3D_FEATURE_LEVEL featureLevel;
		HRESULT result;
		UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;

		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = impl->window.getWidth();
		swapChainDesc.BufferDesc.Height = impl->window.getHeight();
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = impl->hwnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

#ifdef _DEBUG
		flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		result = D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
            flags,
			&featureLevels,
			1,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&impl->swapChain,
			&impl->device,
			&featureLevel,
			&impl->context
			);

		checkError(result);

#ifdef _DEBUG
		// Get pointer to debug.
		impl->device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&impl->debug));
#endif

		// Create depth-stencil buffer.
		D3D11_TEXTURE2D_DESC depthStencilDesc;

		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		depthStencilDesc.Width = swapChainDesc.BufferDesc.Width;
		depthStencilDesc.Height = swapChainDesc.BufferDesc.Height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
        depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		result = impl->device->CreateTexture2D(&depthStencilDesc, NULL, &impl->depthStencilBuffer);

		checkError(result);

		// Create depth-stencil view.
		result = impl->device->CreateDepthStencilView(impl->depthStencilBuffer, NULL, &impl->depthStencilView);

		checkError(result);

		// Create default render target.
		result = impl->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&impl->backBufferTexture);

		checkError(result);

		// Create render target view.
        impl->defaultRenderTarget = new DX11RenderTarget();
        impl->defaultRenderTarget->count = 1;
        impl->defaultRenderTarget->views = new ID3D11RenderTargetView*[1];
        result = impl->device->CreateRenderTargetView(impl->backBufferTexture, NULL, &impl->defaultRenderTarget->views[0]);

		checkError(result);

        bindRenderTarget(&impl->defaultRenderTarget->header);
	}

	void GraphicsDevice::deinit()
	{
		// Set fullscreen to false before releasing the swap chain.
		impl->swapChain->SetFullscreenState(FALSE, NULL);

        impl->defaultRenderTarget->views[0]->Release();
		impl->backBufferTexture->Release();
		impl->depthStencilBuffer->Release();
		impl->depthStencilView->Release();

		impl->swapChain->Release();
		impl->context->Release();
		impl->device->Release();

        delete impl->defaultRenderTarget;

#ifdef _DEBUG
		impl->debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		impl->debug->Release();
#endif
	}

	void GraphicsDevice::swap()
	{
		impl->swapChain->Present(0, 0);
	}

	void GraphicsDevice::clear(float r, float g, float b, float a)
	{
		float clear[] = { r, g, b, a };

        for (size_t i = 0; i < impl->currentRenderTarget->count; i++)
        {
            impl->context->ClearRenderTargetView(impl->currentRenderTarget->views[i], clear);
        }

        impl->context->ClearDepthStencilView(impl->depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	Buffer* GraphicsDevice::createBuffer(BufferType type, BufferUsage usage, size_t size)
	{
		DX11Buffer* dx11Buffer = new DX11Buffer();

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		switch (usage)
		{
		case BufferUsage::DYNAMIC:
			bd.Usage = D3D11_USAGE_DYNAMIC; 
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; 
			break;
		case BufferUsage::STATIC:
			// TODO Fix default usage.
			SGE_ASSERT(false);
			break;
		}

		switch (type)
		{
		case BufferType::VERTEX:
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; break;
		case BufferType::INDEX:
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER; break;
		case BufferType::UNIFORM:
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; break;
		}

		bd.ByteWidth = size;

		impl->device->CreateBuffer(&bd, NULL, &dx11Buffer->buffer);

		dx11Buffer->header.size = size;

		return &dx11Buffer->header;
	}

	void GraphicsDevice::deleteBuffer(Buffer* buffer)
	{
		DX11Buffer* dx11Buffer = reinterpret_cast<DX11Buffer*>(buffer);
		dx11Buffer->buffer->Release();

		delete dx11Buffer;
		buffer = nullptr;
	}

	Pipeline* GraphicsDevice::createPipeline(VertexLayoutDescription* vertexLayoutDescription, Shader* vertexShader, Shader* pixelShader)
	{
		DX11Pipeline* dx11Pipeline = new DX11Pipeline();

		dx11Pipeline->vertexShader = reinterpret_cast<DX11Shader*>(vertexShader);
		dx11Pipeline->pixelShader = reinterpret_cast<DX11Shader*>(pixelShader);

		dx11Pipeline->vertexLayout = new DX11VertexLayout();

		// TODO split different initialization steps to individual functions

		// Create input layout for the pipeline.
		D3D11_INPUT_ELEMENT_DESC* inputLayoutDesc = new D3D11_INPUT_ELEMENT_DESC[vertexLayoutDescription->count];

		size_t positionElements = 0;
		size_t normalElements = 0;
		size_t binormalElements = 0;
		size_t colorElements = 0;
		size_t tangentElements = 0;
		size_t texcoordElements = 0;

		size_t stride = 0;

		for (size_t i = 0; i < vertexLayoutDescription->count; i++)
		{
			inputLayoutDesc[i].InputSlot = 0;
			inputLayoutDesc[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			inputLayoutDesc[i].InstanceDataStepRate = 0;
			inputLayoutDesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

			switch (vertexLayoutDescription->elements[i].size)
			{
			case 2:
				inputLayoutDesc[i].Format = DXGI_FORMAT_R32G32_FLOAT; break;
			case 3:
				inputLayoutDesc[i].Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
			case 4:
				inputLayoutDesc[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
			}

			stride += vertexLayoutDescription->elements[i].size;

			switch (vertexLayoutDescription->elements[i].semantic)
			{
			case VertexSemantic::POSITION: 
				inputLayoutDesc[i].SemanticName = "POSITION";
				inputLayoutDesc[i].SemanticIndex = positionElements++;
				break;
			case VertexSemantic::NORMAL:
				inputLayoutDesc[i].SemanticName = "NORMAL";
				inputLayoutDesc[i].SemanticIndex = normalElements++;
				break;
			case VertexSemantic::BINORMAL:
				inputLayoutDesc[i].SemanticName = "BINORMAL";
				inputLayoutDesc[i].SemanticIndex = binormalElements++;
				break;
			case VertexSemantic::COLOR:
				inputLayoutDesc[i].SemanticName = "COLOR";
				inputLayoutDesc[i].SemanticIndex = colorElements++;
				break;
			case VertexSemantic::TANGENT:
				inputLayoutDesc[i].SemanticName = "TANGENT";
				inputLayoutDesc[i].SemanticIndex = tangentElements++;
				break;
			case VertexSemantic::TEXCOORD:
				inputLayoutDesc[i].SemanticName = "TEXCOORD";
				inputLayoutDesc[i].SemanticIndex = texcoordElements++;
				break;
			}
		}

		dx11Pipeline->vertexLayout->header.stride = stride;
		dx11Pipeline->vertexLayout->header.count = vertexLayoutDescription->count;

		HRESULT result;
		result = impl->device->CreateInputLayout(
			inputLayoutDesc,
			vertexLayoutDescription->count,
			dx11Pipeline->vertexShader->source,
			dx11Pipeline->vertexShader->size,
			&dx11Pipeline->vertexLayout->inputLayout
		);

		checkError(result);

		// Create sampler state.
		D3D11_SAMPLER_DESC samplerDesc;

		ZeroMemory(&samplerDesc, sizeof(samplerDesc));

		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0;
		samplerDesc.MaxAnisotropy = 16;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.BorderColor[0] = 1.0f;
		samplerDesc.BorderColor[1] = 1.0f;
		samplerDesc.BorderColor[2] = 1.0f;
		samplerDesc.BorderColor[3] = 1.0f;
		samplerDesc.MinLOD = -FLT_MAX;
		samplerDesc.MaxLOD = FLT_MAX;

		result = impl->device->CreateSamplerState(
			&samplerDesc,
			&dx11Pipeline->samplerState);

		checkError(result);

		// Create rasterizer state.
		D3D11_RASTERIZER_DESC rasterizerDesc;

		ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));

		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_BACK;
		rasterizerDesc.FrontCounterClockwise = TRUE;

		result = impl->device->CreateRasterizerState(
			&rasterizerDesc,
			&dx11Pipeline->rasterizerState);

		checkError(result);

		// Create depth-stencil state.
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.StencilEnable = TRUE;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		result = impl->device->CreateDepthStencilState(
			&depthStencilDesc, 
			&dx11Pipeline->depthStencilState);

		checkError(result);

		// Create blend state.
		D3D11_BLEND_DESC blendDesc;

		ZeroMemory(&blendDesc, sizeof(blendDesc));

		blendDesc.AlphaToCoverageEnable = TRUE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;

		result = impl->device->CreateBlendState(
			&blendDesc, 
			&dx11Pipeline->blendState);
		
		checkError(result);

		return &dx11Pipeline->header;
	}

	void GraphicsDevice::deletePipeline(Pipeline* pipeline)
	{
		DX11Pipeline* dx11Pipeline = reinterpret_cast<DX11Pipeline*>(pipeline);

		dx11Pipeline->vertexLayout->inputLayout->Release();
		dx11Pipeline->samplerState->Release();
		dx11Pipeline->rasterizerState->Release();
		dx11Pipeline->depthStencilState->Release();
		dx11Pipeline->blendState->Release();

		delete dx11Pipeline->vertexLayout;
		delete dx11Pipeline;

		pipeline = nullptr;
	}

    RenderTarget* GraphicsDevice::createRenderTarget(size_t count, Texture** textures)
    {
        SGE_ASSERT(count <= D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT);

        DX11RenderTarget* dx11RenderTarget = new DX11RenderTarget();

        HRESULT result = S_OK;

        dx11RenderTarget->views = new ID3D11RenderTargetView*[count];
        dx11RenderTarget->count = count;

        for (size_t i = 0; i < count; i++)
        {
            result = impl->device->CreateRenderTargetView(reinterpret_cast<DX11Texture*>(textures[i])->texture, NULL, &dx11RenderTarget->views[i]);

            checkError(result);
        }

        return &dx11RenderTarget->header;
    }

    void GraphicsDevice::deleteRenderTarget(RenderTarget* renderTarget)
    {
        DX11RenderTarget* dx11RenderTarget = reinterpret_cast<DX11RenderTarget*>(renderTarget);

        for (size_t i = 0; i < dx11RenderTarget->count; i++)
        {
            dx11RenderTarget->views[i]->Release();
        }

        delete dx11RenderTarget;

        renderTarget = nullptr;
    }

	Shader* GraphicsDevice::createShader(ShaderType type, const char* source, size_t size)
	{
		DX11Shader* dx11Shader = new DX11Shader();
		HRESULT result = S_OK;

		switch (type)
		{
		case ShaderType::VERTEX: 
			result = impl->device->CreateVertexShader(source, size, NULL, &dx11Shader->vertexShader); break;
		case ShaderType::PIXEL: 
			result = impl->device->CreatePixelShader(source, size, NULL, &dx11Shader->pixelShader); break;
		}

		checkError(result);

		dx11Shader->header.type = type;
		dx11Shader->source = source;
		dx11Shader->size = size;

		return &dx11Shader->header;
	}

	void GraphicsDevice::deleteShader(Shader* shader)
	{
		DX11Shader* dx11Shader = reinterpret_cast<DX11Shader*>(shader);

		switch (dx11Shader->header.type)
		{
		case ShaderType::VERTEX: 
			dx11Shader->vertexShader->Release(); break;
		case ShaderType::PIXEL:
			dx11Shader->pixelShader->Release(); break;
		}

		shader = nullptr;
	}

	Texture* GraphicsDevice::createTexture(size_t width, size_t height, unsigned char* source)
	{
		DX11Texture* dx11Texture = new DX11Texture();

		D3D11_TEXTURE2D_DESC textureDesc;
		HRESULT result = S_OK;

		ZeroMemory(&textureDesc, sizeof(textureDesc));

		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;


        if (source)
        {
            D3D11_SUBRESOURCE_DATA data;
            data.pSysMem = source;
            data.SysMemPitch = static_cast<UINT>(width * 4);
            data.SysMemSlicePitch = 0;

            result = impl->device->CreateTexture2D(&textureDesc, &data, &dx11Texture->texture);
        }
        else
        {
            result = impl->device->CreateTexture2D(&textureDesc, NULL, &dx11Texture->texture);
        }

		checkError(result);

        result = impl->device->CreateShaderResourceView(dx11Texture->texture, nullptr, &dx11Texture->view);

		checkError(result);

		impl->context->GenerateMips(dx11Texture->view);

		return &dx11Texture->header;
	}


	Texture* GraphicsDevice::createTextTexture(size_t width, size_t height, unsigned char* source)
	{
		return nullptr;

		DX11Texture* dx11Texture = new DX11Texture();

		D3D11_TEXTURE2D_DESC textureDesc;
		HRESULT result = S_OK;

		ZeroMemory(&textureDesc, sizeof(textureDesc));

		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;


		if (source)
		{
			D3D11_SUBRESOURCE_DATA data;
			data.pSysMem = source;
			data.SysMemPitch = static_cast<UINT>(width * 4);
			data.SysMemSlicePitch = 0;

			result = impl->device->CreateTexture2D(&textureDesc, &data, &dx11Texture->texture);
		}
		else
		{
			result = impl->device->CreateTexture2D(&textureDesc, NULL, &dx11Texture->texture);
		}

		checkError(result);

		result = impl->device->CreateShaderResourceView(dx11Texture->texture, nullptr, &dx11Texture->view);

		checkError(result);

		impl->context->GenerateMips(dx11Texture->view);

		return &dx11Texture->header;
	}

	void GraphicsDevice::deleteTexture(Texture* texture)
	{
		DX11Texture* dx11Texture = reinterpret_cast<DX11Texture*>(texture);

		dx11Texture->texture->Release();
        dx11Texture->view->Release();

		delete dx11Texture;
		texture = nullptr;
	}

	void GraphicsDevice::bindPipeline(Pipeline* pipeline)
	{
		DX11Pipeline* dx11Pipeline = reinterpret_cast<DX11Pipeline*>(pipeline);
		impl->context->IASetInputLayout(dx11Pipeline->vertexLayout->inputLayout);
		impl->context->VSSetShader(dx11Pipeline->vertexShader->vertexShader, 0, 0);
		impl->context->PSSetShader(dx11Pipeline->pixelShader->pixelShader, 0, 0);
		impl->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		impl->context->PSSetSamplers(0, 1, &dx11Pipeline->samplerState);
		impl->context->RSSetState(dx11Pipeline->rasterizerState);
		impl->context->OMSetDepthStencilState(dx11Pipeline->depthStencilState, 0);
		impl->context->OMSetBlendState(dx11Pipeline->blendState, NULL, 0xffffffff);

		impl->pipeline = dx11Pipeline;
	}

	void GraphicsDevice::debindPipeline(Pipeline* pipeline)
	{

	}

    void GraphicsDevice::bindRenderTarget(RenderTarget* renderTarget)
    {
        DX11RenderTarget* dx11RenderTarget = reinterpret_cast<DX11RenderTarget*>(renderTarget);

        impl->currentRenderTarget = dx11RenderTarget;
        impl->context->OMSetRenderTargets(dx11RenderTarget->count, dx11RenderTarget->views, NULL);
    }

    void GraphicsDevice::debindRenderTarget()
    {
        bindRenderTarget(&impl->defaultRenderTarget->header);
    }

	void GraphicsDevice::bindVertexBuffer(Buffer* buffer)
	{
		DX11Buffer* dx11Buffer = reinterpret_cast<DX11Buffer*>(buffer);

		// TODO do we really need to do this?
		UINT stride = impl->pipeline->vertexLayout->header.stride * sizeof(float);
		UINT offset = 0;

		impl->context->IASetVertexBuffers(0, 1, &dx11Buffer->buffer, &stride, &offset);
	}

	void GraphicsDevice::bindIndexBuffer(Buffer* buffer)
	{
		impl->context->IASetIndexBuffer(reinterpret_cast<DX11Buffer*>(buffer)->buffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void GraphicsDevice::bindVertexUniformBuffer(Buffer* buffer, size_t slot)
	{
		impl->context->VSSetConstantBuffers(slot, 1, &reinterpret_cast<DX11Buffer*>(buffer)->buffer);
	}

	void GraphicsDevice::bindPixelUniformBuffer(Buffer* buffer, size_t slot)
	{
		impl->context->PSSetConstantBuffers(slot, 1, &reinterpret_cast<DX11Buffer*>(buffer)->buffer);
	}

	void GraphicsDevice::bindViewport(Viewport* viewport)
	{
		D3D11_VIEWPORT d11Viewport;
		ZeroMemory(&d11Viewport, sizeof(D3D11_VIEWPORT));

		d11Viewport.Width = static_cast<FLOAT>(viewport->width);
		d11Viewport.Height = static_cast<FLOAT>(viewport->height);
		d11Viewport.MinDepth = 0.0f;
		d11Viewport.MaxDepth = 1.0f;
		d11Viewport.TopLeftX = static_cast<FLOAT>(viewport->x);
		d11Viewport.TopLeftY = static_cast<FLOAT>(impl->window.getHeight() - viewport->y - viewport->height);

		impl->context->RSSetViewports(1, &d11Viewport);
	}

	void GraphicsDevice::bindTexture(Texture* texture, size_t slot)
	{
        impl->context->PSSetShaderResources(slot, 1, &reinterpret_cast<DX11Texture*>(texture)->view);
	}

	void GraphicsDevice::debindTexture(Texture* texture, size_t slot)
	{
        ID3D11ShaderResourceView* tab[] = { NULL };
        impl->context->PSSetShaderResources(slot, 1, tab);
	}

	void GraphicsDevice::copyData(Buffer* buffer, size_t size, const void* data)
	{
		DX11Buffer* dx11Buffer = reinterpret_cast<DX11Buffer*>(buffer);

		D3D11_MAPPED_SUBRESOURCE ms;
		impl->context->Map(dx11Buffer->buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, data, size);
		impl->context->Unmap(dx11Buffer->buffer, NULL);
	}

	void GraphicsDevice::copySubData(Buffer* buffer, size_t offset, size_t size, const void* data)
	{

	}

	void GraphicsDevice::draw(size_t count)
	{
		impl->context->Draw(count, 0);
	}

	void GraphicsDevice::drawIndexed(size_t count)
	{
		impl->context->DrawIndexed(count, 0, 0);
	}

	void GraphicsDevice::drawInstanced(size_t count, size_t instanceCount)
	{

	}

	void GraphicsDevice::drawInstancedIndexed(size_t count, size_t instanceCount)
	{

	}
}
#endif