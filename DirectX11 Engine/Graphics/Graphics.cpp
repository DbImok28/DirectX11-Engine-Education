#include "Graphics.hpp"
#include "../Paths.hpp"

bool Graphics::Initialize(HWND hWnd, int width, int height)
{
	if (!InitializeDirectX(hWnd, width, height))
		return false;
	if (!InitializeShader())
		return false;
	return true;
}
/*
*1  Input Assembler		(IA) Stage
*2  Vertex Shader		(VS) Stage
3  Hull Shader			(HS) Stage
4  Tessellator Shader	(TS) Stage
5  Domain Shader		(DS) Stage
6  Geometry Shader		(GS) Stage
7  Stream Output		(SO) Stage
8  Rasterizer			(RS) Stage
9  Pixel Shader			(PS) Stage
10 Output Merger		(OM) Stage
*/
void Graphics::RenderFrame()
{
	float bgcolor[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), bgcolor);
	swapchain->Present(1, NULL);
}

bool Graphics::InitializeDirectX(HWND hWnd, int width, int height)
{
	std::vector<AdapterData> adapters = AdapterReader::GetAdapters();
	if (adapters.size() < 1)
	{
		ErrorLogger::Log("No DXGI Adapters found.");
		return false;
	}

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hWnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		adapters[0].pAdapter,			// Adapter
		D3D_DRIVER_TYPE_UNKNOWN,		// DriverType
		NULL,							// Software
		NULL,							// Flags
		NULL,							// FeatureLevels
		0,								// FeatureLevels
		D3D11_SDK_VERSION,				// SDKVersion
		&scd,							// SwapChainDesc
		swapchain.GetAddressOf(),		// SwapChain
		device.GetAddressOf(),			// Device
		NULL,							// FeatureLevel
		deviceContext.GetAddressOf()	// ImmediateContext
	);
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create device and swapchain.");
		return false;
	}
	
	com_ptr<ID3D11Texture2D> backBuffer;
	hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "GetBuffer. Failed to create backBuffer.");
		return false;
	}

	hr = device->CreateRenderTargetView(backBuffer.Get(), NULL, renderTargetView.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create RenderTargetView.");
		return false;
	}

	this->deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), NULL);

	return true;
}

bool Graphics::InitializeShader()
{
	D3D11_INPUT_ELEMENT_DESC loyout[]
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(loyout);

	if (!vertexShader.Initalize(device, Paths::ShaderFolder + L"VertexShader.cso", loyout, numElements))
	{
		return false;
	}
	
	return true;
}