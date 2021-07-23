#include "Graphics.hpp"

bool Graphics::Initialize(HWND hWnd, int width, int height)
{
	fpsTimer.Start();
	windowWidth = width;
	windowHeight = height;
	if (!InitializeDirectX(hWnd))
		return false;
	if (!InitializeShader())
		return false;
	if (!InitializeScene())
		return false;

	// Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device.Get(), deviceContext.Get());
	ImGui::StyleColorsDark();
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
*8  Rasterizer			(RS) Stage
9  Pixel Shader			(PS) Stage
*10 Output Merger		(OM) Stage
*/
void Graphics::RenderFrame()
{
	float bgcolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), bgcolor);
	deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	deviceContext->IASetInputLayout(vertexShader.GatInputLoyout());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//deviceContext->RSSetState(rasterizerState.Get());
	deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);
	deviceContext->OMSetBlendState(blendState.Get(), NULL, 0xFFFFFFFF);

	deviceContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());
	deviceContext->VSSetShader(vertexShader.GetShader(), NULL, 0);
	deviceContext->PSSetShader(pixelShader.GetShader(), NULL, 0);
	





	// world;
	//XMMATRIX world = XMMatrixIdentity();
	static float alpha = 0.5f;


	//{ // MyTexture
	//	static float translationOffset[3]{ 0,0,4.0f };
	//	XMMATRIX world = XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(translationOffset[0], translationOffset[1], translationOffset[2]);

	//	// Constant buffer
	//	cb_vs_VertexShader.data.mat = world * camera.GetViewMatrix() * camera.GetProjectionMatrix();
	//	cb_vs_VertexShader.data.mat = XMMatrixTranspose(cb_vs_VertexShader.data.mat);
	//	if (!cb_vs_VertexShader.ApplyChanges()) return;
	//	deviceContext->VSSetConstantBuffers(0, 1, cb_vs_VertexShader.GetAddressOf());

	//	cb_ps_PixelShader.data.alpha = 1.0f;
	//	if (!cb_ps_PixelShader.ApplyChanges()) return;
	//	deviceContext->PSSetConstantBuffers(0, 1, cb_ps_PixelShader.GetAddressOf());

	//	UINT offset = 0;
	//	deviceContext->PSSetShaderResources(0, 1, myTexture.GetAddressOf());
	//	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
	//	deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	//	deviceContext->DrawIndexed(indexBuffer.BufferSize(), 0, 0);
	//}
	{ // Concrete
		static float translationOffset[3]{ 0,0,-2 };
		XMMATRIX world = XMMatrixTranslation(translationOffset[0], translationOffset[1], translationOffset[2]);

		// Constant buffer
		cb_vs_VertexShader.data.mat = world * camera.GetViewMatrix() * camera.GetProjectionMatrix();
		cb_vs_VertexShader.data.mat = XMMatrixTranspose(cb_vs_VertexShader.data.mat);
		if (!cb_vs_VertexShader.ApplyChanges()) return;
		deviceContext->VSSetConstantBuffers(0, 1, cb_vs_VertexShader.GetAddressOf());

		cb_ps_PixelShader.data.alpha = alpha;
		if (!cb_ps_PixelShader.ApplyChanges()) return;
		deviceContext->PSSetConstantBuffers(0, 1, cb_ps_PixelShader.GetAddressOf());

		UINT offset = 0;
		deviceContext->PSSetShaderResources(0, 1, concreteTexture.GetAddressOf());
		deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
		deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

		//deviceContext->RSSetState(rasterizerState_CullFront.Get());
		//deviceContext->DrawIndexed(indexBuffer.BufferSize(), 0, 0);
		deviceContext->RSSetState(rasterizerState.Get());
		deviceContext->DrawIndexed(indexBuffer.BufferSize(), 0, 0);
	}
	{ // Grass
		static float translationOffset[3]{ 2,0,-2 };
		XMMATRIX world = XMMatrixTranslation(translationOffset[0], translationOffset[1], translationOffset[2]);

		// Constant buffer
		cb_vs_VertexShader.data.mat = world * camera.GetViewMatrix() * camera.GetProjectionMatrix();
		cb_vs_VertexShader.data.mat = XMMatrixTranspose(cb_vs_VertexShader.data.mat);
		if (!cb_vs_VertexShader.ApplyChanges()) return;
		deviceContext->VSSetConstantBuffers(0, 1, cb_vs_VertexShader.GetAddressOf());

		cb_ps_PixelShader.data.alpha = 0.5f;
		if (!cb_ps_PixelShader.ApplyChanges()) return;
		deviceContext->PSSetConstantBuffers(0, 1, cb_ps_PixelShader.GetAddressOf());

		UINT offset = 0;
		deviceContext->PSSetShaderResources(0, 1, grassTexture.GetAddressOf());
		deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
		deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
		deviceContext->DrawIndexed(indexBuffer.BufferSize(), 0, 0);
	}






	// fps
	static int fpsCount = 0;
	static std::string fpsStr = "FPS: 0";
	fpsCount++;
	if (fpsTimer.GetMilisecondsElapsed() / 1000 > 1)
	{
		fpsStr = "FPS: " + std::to_string(fpsCount);
		fpsCount = 0;
		fpsTimer.Restart();
	}

	// Draw text
	spriteBatch->Begin();
	spriteFont->DrawString(spriteBatch.get(), StringConverter::StringToWide(fpsStr).c_str(), DirectX::XMFLOAT2(0.0f, 0.0f),  DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
	spriteBatch->End();

	// Start ImGui frame
	static bool VerticalSync = true;
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Test");
	ImGui::DragFloat("Alpha", &alpha, 0.01f, 0.0f, 1.0f);
	ImGui::End();
	ImGui::Begin("Settings");
	ImGui::Checkbox("VSync", &VerticalSync);
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swapchain->Present(VerticalSync, NULL);


}

bool Graphics::InitializeDirectX(HWND hWnd)
{
	std::vector<AdapterData> adapters = AdapterReader::GetAdapters();
	if (adapters.size() < 1)
	{
		ErrorLogger::Log("No DXGI Adapters found.");
		return false;
	}

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Width = windowWidth;
	scd.BufferDesc.Height = windowHeight;
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
		ErrorLogger::Log(hr, "GetBuffer. Failed to create back buffer.");
		return false;
	}

	hr = device->CreateRenderTargetView(backBuffer.Get(), NULL, renderTargetView.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create render target view.");
		return false;
	}

	// Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = windowWidth;
	depthStencilDesc.Height = windowHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	hr = device->CreateTexture2D(&depthStencilDesc, NULL, depthStencilBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create depth stencil buffer.");
		return false;
	}

	hr = device->CreateDepthStencilView(depthStencilBuffer.Get(), NULL, depthStencilView.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create depth stencil view.");
		return false;
	}

	this->deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

	// Depth Stencil State
	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilStateDesc.DepthEnable = true;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
	hr = device->CreateDepthStencilState(&depthStencilStateDesc, depthStencilState.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create depth stencil state.");
		return false;
	}

	// Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<FLOAT>(windowWidth);
	viewport.Height = static_cast<FLOAT>(windowHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	deviceContext->RSSetViewports(1, &viewport);

	// Rasterizer state
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID; // Fill the triangles formed by the vertices. Adjacent vertices are not drawn.
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;	 // Do not draw triangles that are back-facing.
	hr = device->CreateRasterizerState(&rasterizerDesc, rasterizerState.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create rasterizer state.");
		return false;
	}
	// Rasterizer state Cull front
	//D3D11_RASTERIZER_DESC rasterizerCullDesc;
	//ZeroMemory(&rasterizerCullDesc, sizeof(D3D11_RASTERIZER_DESC));
	//rasterizerCullDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID; // Fill the triangles formed by the vertices. Adjacent vertices are not drawn.
	//rasterizerCullDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;	 // Do not draw triangles that are back-facing.
	//hr = device->CreateRasterizerState(&rasterizerCullDesc, rasterizerState_CullFront.GetAddressOf());
	//if (FAILED(hr))
	//{
	//	ErrorLogger::Log(hr, "Failed to create rasterizer state.");
	//	return false;
	//}

	// Blend state
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbDesc;
	ZeroMemory(&rtbDesc, sizeof(rtbDesc));
	rtbDesc.BlendEnable = TRUE;
	// color
	rtbDesc.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	rtbDesc.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	rtbDesc.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	// alpha
	rtbDesc.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	rtbDesc.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
	rtbDesc.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	rtbDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.RenderTarget[0] = rtbDesc;
	hr = device->CreateBlendState(&blendDesc, blendState.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create blend state.");
		return false;
	}


	// Text
	spriteBatch = std::make_unique<DirectX::SpriteBatch>(deviceContext.Get());
	spriteFont = std::make_unique<DirectX::SpriteFont>(device.Get(), L"Data\\Fonts\\comic_sans_ms_16.spritefont");

	// Sampler state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = device->CreateSamplerState(&sampDesc, samplerState.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create sampler state.");
		return false;
	}

	return true;
}

bool Graphics::InitializeShader()
{
	D3D11_INPUT_ELEMENT_DESC loyout[]
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(loyout);

	if (!vertexShader.Initalize(device, Paths::ShaderFolder + L"VertexShader.cso", loyout, numElements))
		return false;

	if (!pixelShader.Initalize(device, Paths::ShaderFolder + L"PixelShader.cso"))
		return false;
	
	return true;
}

bool Graphics::InitializeScene()
{
	// Vertices
	Vertex v[]
	{
		{-0.5f, -0.5f, -0.5f, 0.0f, 1.0f}, // front
		{-0.5f,  0.5f, -0.5f, 0.0f, 0.0f},
		{ 0.5f,  0.5f, -0.5f, 1.0f, 0.0f},
		{ 0.5f, -0.5f, -0.5f, 1.0f, 1.0f},

		{-0.5f, -0.5f, 0.5f, 0.0f, 1.0f}, // back
		{-0.5f,  0.5f, 0.5f, 0.0f, 0.0f},
		{ 0.5f,  0.5f, 0.5f, 1.0f, 0.0f},
		{ 0.5f, -0.5f, 0.5f, 1.0f, 1.0f},
	};

	HRESULT hr = vertexBuffer.Initialize(device.Get(), v, ARRAYSIZE(v));
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create vertex buffer.");
		return false;
	}

	// Indices
	DWORD indices[] =
	{
		0, 1, 2, 0, 2, 3, // front
		4, 7, 6, 4, 6, 5, // back
		3, 2, 6, 3, 6, 7, // right
		4, 5, 1, 4, 1, 0, // left
		1, 5, 6, 1, 6, 2, // top
		0, 3, 7, 0, 7, 4, // bottom
	};
	hr = indexBuffer.Initialize(device.Get(), indices, ARRAYSIZE(indices));
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create index buffer.");
		return false;
	}

	// load texture
	hr = DirectX::CreateWICTextureFromFile(device.Get(), L"Data\\Textures\\myImg.jpg", nullptr, myTexture.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to load texture from file.");
		return false;
	}
	hr = DirectX::CreateWICTextureFromFile(device.Get(), L"Data\\Textures\\concrete.jpg", nullptr, concreteTexture.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to load texture from file.");
		return false;
	}
	hr = DirectX::CreateWICTextureFromFile(device.Get(), L"Data\\Textures\\grass.jpg", nullptr, grassTexture.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to load texture from file.");
		return false;
	}

	// Constant buffer
	hr = cb_vs_VertexShader.Initialize(device.Get(), deviceContext.Get());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create constant buffer(cb_vs_VertexShader).");
		return false;
	}
	hr = cb_ps_PixelShader.Initialize(device.Get(), deviceContext.Get());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create constant buffer(cb_ps_PixelShader).");
		return false;
	}

	float nearZ = 0.1f;
	float farZ = 10000.0f;
	camera.SetPosition(0.0f, 0.0f, -2.0f);
	camera.SetProjectionValues(90.0f, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), nearZ, farZ);

	return true;
}