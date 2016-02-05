#ifdef DIRECTX11

#include <Windows.h>
#include <d3d11.h>

#pragma comment (lib, "d3d11.lib")

#include <iostream>
#include "SDL/SDL_syswm.h"

#include "Core/Assert.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/Window.h"

namespace sge
{
	struct GraphicsDevice::Impl
	{
		Impl(Window& window) : 
			hdc(nullptr), 
			hwnd(nullptr),
			device(nullptr),
			context(nullptr),
			swapChain(nullptr),
			renderTargetView(nullptr)
		{
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
				}
				default: break;
				}
			}

			std::cout << "SDL version " << 
				static_cast<int>(info.version.major) << "." << 
				static_cast<int>(info.version.minor) << "." << 
				static_cast<int>(info.version.patch) << std::endl;

			ZeroMemory(&sd, sizeof(sd));

			sd.BufferCount = 1;
			sd.BufferDesc.Width = window.getWidth();
			sd.BufferDesc.Height = window.getHeight();
			sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.BufferDesc.RefreshRate.Numerator = 60;
			sd.BufferDesc.RefreshRate.Denominator = 1;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.OutputWindow = hwnd;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.Windowed = true;

			HRESULT result = D3D11CreateDeviceAndSwapChain(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				D3D11_CREATE_DEVICE_SINGLETHREADED || D3D11_CREATE_DEVICE_DEBUG || D3D11_CREATE_DEVICE_DEBUGGABLE,
				&featureLevels,
				1,
				D3D11_SDK_VERSION,
				&sd,
				&swapChain,
				&device,
				&featureLevel,
				&context
				);

			SGE_ASSERT(result == S_OK);

			ID3D11Texture2D* backBuffer = nullptr;
			result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);

			SGE_ASSERT(result == S_OK);

			device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
			context->OMSetRenderTargets(1, &renderTargetView, nullptr);
			backBuffer->Release();

			viewport.Width = window.getWidth();
			viewport.Height = window.getHeight();
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;

			context->RSSetViewports(1, &viewport);
		}

		~Impl()
		{
			swapChain->Release();
			device->Release();
			context->Release();
			renderTargetView->Release();
		}

		HDC hdc;
		HWND hwnd;
		ID3D11Device* device;
		ID3D11DeviceContext* context;
		IDXGISwapChain* swapChain;
		ID3D11RenderTargetView* renderTargetView;
		D3D_FEATURE_LEVEL featureLevel;
		D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_11_0;
		DXGI_SWAP_CHAIN_DESC sd;
		D3D11_VIEWPORT viewport;
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

	}

	void GraphicsDevice::deinit()
	{

	}

	void GraphicsDevice::swap()
	{
		impl->swapChain->Present(0, 0);
	}

	void GraphicsDevice::clear(float r, float g, float b, float a)
	{
		float clear[] = { r, g, b, a };
		impl->context->ClearRenderTargetView(impl->renderTargetView, clear);
	}

	Buffer* GraphicsDevice::createBuffer(BufferType type, BufferUsage usage)
	{
		return nullptr;
	}

	void GraphicsDevice::deleteBuffer(Buffer* buffer)
	{

	}

	Pipeline* GraphicsDevice::createPipeline(VertexLayoutDescription* vertexLayoutDescription, Shader* vertexShader, Shader* pixelShader)
	{
		return nullptr;
	}

	void GraphicsDevice::deletePipeline(Pipeline* pipeline)
	{

	}

	Shader* GraphicsDevice::createShader(ShaderType type, const char* source)
	{
		return nullptr;
	}

	void GraphicsDevice::deleteShader(Shader* shader)
	{

	}

	Texture* GraphicsDevice::createTexture(size_t width, size_t height, unsigned char* source)
	{
		return nullptr;
	}

	void GraphicsDevice::deleteTexture(Texture* texture)
	{

	}

	void GraphicsDevice::bindPipeline(Pipeline* pipeline)
	{

	}

	void GraphicsDevice::debindPipeline(Pipeline* pipeline)
	{

	}

	void GraphicsDevice::bindVertexBuffer(Buffer* buffer)
	{

	}

	void GraphicsDevice::bindIndexBuffer(Buffer* buffer)
	{

	}

	void GraphicsDevice::bindVertexUniformBuffer(Buffer* buffer, size_t slot)
	{

	}

	void GraphicsDevice::bindPixelUniformBuffer(Buffer* buffer, size_t slot)
	{

	}

	void GraphicsDevice::bindViewport(Viewport* viewport)
	{

	}

	void GraphicsDevice::bindTexture(Texture* texture, size_t slot)
	{

	}

	void GraphicsDevice::debindTexture(Texture* texture)
	{

	}

	void GraphicsDevice::copyData(Buffer* buffer, size_t size, const void* data)
	{

	}

	void GraphicsDevice::copySubData(Buffer* buffer, size_t offset, size_t size, const void* data)
	{

	}

	void GraphicsDevice::draw(size_t count)
	{

	}

	void GraphicsDevice::drawIndexed(size_t count)
	{

	}

	void GraphicsDevice::drawInstanced(size_t count, size_t instanceCount)
	{

	}

	void GraphicsDevice::drawInstancedIndexed(size_t count, size_t instanceCount)
	{

	}
}
#endif