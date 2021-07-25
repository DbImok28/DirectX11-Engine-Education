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
	cb_ps_Light.data.dynamicLightColor = light.lightColor;
	cb_ps_Light.data.dynamicLightStrength = light.lightStrength;
	cb_ps_Light.data.dynamicLightPosition = light.GetPositionFloat3();
	cb_ps_Light.data.dynamicLightAttenuation_a = light.attenuation_a;
	cb_ps_Light.data.dynamicLightAttenuation_b = light.attenuation_b;
	cb_ps_Light.data.dynamicLightAttenuation_c = light.attenuation_c;


	if (!cb_ps_Light.ApplyChanges()) return;
	deviceContext->PSSetConstantBuffers(0, 1, cb_ps_Light.GetAddressOf());

	float bgcolor[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), bgcolor);
	deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	deviceContext->IASetInputLayout(vertexShader.GatInputLoyout());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->RSSetState(rasterizerState.Get());
	deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);
	//deviceContext->OMSetBlendState(blendState.Get(), NULL, 0xFFFFFFFF);
	deviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);
	deviceContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());
	deviceContext->VSSetShader(vertexShader.GetShader(), NULL, 0);
	deviceContext->PSSetShader(pixelShader.GetShader(), NULL, 0);

	XMMATRIX viewProjectonMatrix = camera.GetViewMatrix() * camera.GetProjectionMatrix();
	static float pos[3] = { 0.0f, 0.0f, 0.0f };
	static float rot[3] = { 0.0f, 0.0f, 0.0f };

	{ // Concrete
		gameObject.SetPosition(pos[0], pos[1], pos[2]);
		gameObject.SetRotation(rot[0], rot[1], rot[2]);
		gameObject.Draw(viewProjectonMatrix);
	}


	//static float lpos[3] = { 0.0f, 0.0f, 0.0f };
	{ // light
		deviceContext->PSSetShader(pixelShaderNoLight.GetShader(), NULL, 0);
		light.Draw(viewProjectonMatrix);
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
	spriteFont->DrawString(spriteBatch.get(), StringHelper::StringToWide(fpsStr).c_str(), DirectX::XMFLOAT2(0.0f, 0.0f),  DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
	spriteBatch->End();

	// Start ImGui frame
	static bool VerticalSync = true;
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Test");
	ImGui::DragFloat3("pos", pos, 0.1f);
	ImGui::DragFloat3("rot", rot, 0.1f);
	ImGui::NewLine();
	//ImGui::DragFloat3("light pos", lpos, 0.1f);
	ImGui::End();

	ImGui::Begin("Light");
	ImGui::NewLine();
	ImGui::DragFloat3("Ambient Light Color", &cb_ps_Light.data.ambientLightColor.x, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Ambient Light Strength", &cb_ps_Light.data.ambientLightStrength, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Strength", &light.lightStrength, 0.01f, 0.1f, 1000.0f);
	ImGui::DragFloat("AttenuationA", &light.attenuation_a, 0.01f, 0.1f,100.0f);
	ImGui::DragFloat("AttenuationB", &light.attenuation_b, 0.01f, 0.0f,100.0f);
	ImGui::DragFloat("AttenuationC", &light.attenuation_c, 0.01f, 0.0f,100.0f);
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
	try
	{
		std::vector<AdapterData> adapters = AdapterReader::GetAdapters();
		if (adapters.size() < 1)
		{
			ErrorLogger::Log("No DXGI Adapters found.");
			return false;
		}

		DXGI_SWAP_CHAIN_DESC scd = { 0 };

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
		COM_ERROR_IF(hr, "Failed to create device and swapchain.");

		com_ptr<ID3D11Texture2D> backBuffer;
		hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		COM_ERROR_IF(hr, "Failed to create back buffer.");

		hr = device->CreateRenderTargetView(backBuffer.Get(), NULL, renderTargetView.GetAddressOf());
		COM_ERROR_IF(hr, "Failed to create render target view.");

		// Depth/Stencil Buffer
		/*D3D11_TEXTURE2D_DESC depthStencilTeaxtureDesc;
		depthStencilTeaxtureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilTeaxtureDesc.Width = windowWidth;
		depthStencilTeaxtureDesc.Height = windowHeight;
		depthStencilTeaxtureDesc.ArraySize = 1;
		depthStencilTeaxtureDesc.MipLevels = 1;
		depthStencilTeaxtureDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
		depthStencilTeaxtureDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		depthStencilTeaxtureDesc.CPUAccessFlags = 0;
		depthStencilTeaxtureDesc.SampleDesc.Count = 1;
		depthStencilTeaxtureDesc.SampleDesc.Quality = 0;
		depthStencilTeaxtureDesc.MiscFlags = 0;*/
		CD3D11_TEXTURE2D_DESC depthStencilTeaxtureDesc(
			DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT,
			windowWidth, windowHeight,1,1);
		depthStencilTeaxtureDesc.MipLevels = 1;
		depthStencilTeaxtureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		hr = device->CreateTexture2D(&depthStencilTeaxtureDesc, NULL, depthStencilBuffer.GetAddressOf());
		COM_ERROR_IF(hr, "Failed to create depth stencil buffer.");

		hr = device->CreateDepthStencilView(depthStencilBuffer.Get(), NULL, depthStencilView.GetAddressOf());
		COM_ERROR_IF(hr, "Failed to create depth stencil view.");
		this->deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

		// Depth Stencil State
		/*D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc = {0};
		depthStencilStateDesc.DepthEnable = TRUE;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;*/
		CD3D11_DEPTH_STENCIL_DESC depthStencilStateDesc(D3D11_DEFAULT);
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		hr = device->CreateDepthStencilState(&depthStencilStateDesc, depthStencilState.GetAddressOf());
		COM_ERROR_IF(hr, "Failed to create depth stencil state.");

		// Viewport
		/*D3D11_VIEWPORT viewport = { 0 };
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<FLOAT>(windowWidth);
		viewport.Height = static_cast<FLOAT>(windowHeight);
		viewport.MinDepth = D3D11_MIN_DEPTH;
		viewport.MaxDepth = D3D11_MAX_DEPTH;*/
		CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<FLOAT>(windowWidth), static_cast<FLOAT>(windowHeight));
		deviceContext->RSSetViewports(1, &viewport);

		// Rasterizer state
		/*D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID; // Fill the triangles formed by the vertices. Adjacent vertices are not drawn.
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;	 // Do not draw triangles that are back-facing.*/
		CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);
		hr = device->CreateRasterizerState(&rasterizerDesc, rasterizerState.GetAddressOf());
		COM_ERROR_IF(hr, "Failed to create rasterizer state.");

		// Rasterizer state Cull front
		//CD3D11_RASTERIZER_DESC rasterizerCullDesc(D3D11_DEFAULT);
		//rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		//hr = device->CreateRasterizerState(&rasterizerCullDesc, rasterizerState_CullFront.GetAddressOf());
		//COM_ERROR_IF(hr, "Failed to create rasterizer state.");

		// Blend state
		D3D11_BLEND_DESC blendDesc = { 0 };

		D3D11_RENDER_TARGET_BLEND_DESC rtbDesc = { 0 };
		rtbDesc.BlendEnable = TRUE;
		rtbDesc.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;		// color
		rtbDesc.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;	// color
		rtbDesc.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;		// color
		rtbDesc.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;		// alpha
		rtbDesc.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;		// alpha
		rtbDesc.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;	// alpha
		rtbDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;
		blendDesc.RenderTarget[0] = rtbDesc;
		hr = device->CreateBlendState(&blendDesc, blendState.GetAddressOf());
		COM_ERROR_IF(hr, "Failed to create blend state.");

		// Text
		spriteBatch = std::make_unique<DirectX::SpriteBatch>(deviceContext.Get());
		spriteFont = std::make_unique<DirectX::SpriteFont>(device.Get(), L"Data\\Fonts\\comic_sans_ms_16.spritefont");

		// Sampler state
		/*D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;*/
		CD3D11_SAMPLER_DESC sampDesc(D3D11_DEFAULT);
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		hr = device->CreateSamplerState(&sampDesc, samplerState.GetAddressOf());
		COM_ERROR_IF(hr, "Failed to create sampler state.");

	}
	catch (COMException& exception)
	{
		ErrorLogger::Log(exception);
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
		{"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(loyout);

	if (!vertexShader.Initalize(device, Paths::ShaderFolder + L"VertexShader.cso", loyout, numElements))
		return false;

	if (!pixelShader.Initalize(device, Paths::ShaderFolder + L"PixelShader.cso"))
		return false;
	if (!pixelShaderNoLight.Initalize(device, Paths::ShaderFolder + L"PixelShader_NoLight.cso"))
		return false;
	
	return true;
}

bool Graphics::InitializeScene()
{
	try
	{
		// load texture
		HRESULT hr = DirectX::CreateWICTextureFromFile(device.Get(), L"Data\\Textures\\myImg.jpg", nullptr, myTexture.GetAddressOf());
		COM_ERROR_IF(hr, "Failed to load texture from file.");

		hr = DirectX::CreateWICTextureFromFile(device.Get(), L"Data\\Textures\\concrete.jpg", nullptr, concreteTexture.GetAddressOf());
		COM_ERROR_IF(hr, "Failed to load texture from file.");

		hr = DirectX::CreateWICTextureFromFile(device.Get(), L"Data\\Textures\\grass.jpg", nullptr, grassTexture.GetAddressOf());
		COM_ERROR_IF(hr, "Failed to load texture from file.");

		// Constant buffer
		hr = cb_vs_VertexShader.Initialize(device.Get(), deviceContext.Get());
		COM_ERROR_IF(hr, "Failed to initialize constant buffer(cb_vs_VertexShader).");

		hr = cb_ps_Light.Initialize(device.Get(), deviceContext.Get());
		COM_ERROR_IF(hr, "Failed to initialize constant buffer(cb_ps_Light).");
		cb_ps_Light.data.ambientLightColor = { 1.0f,1.0f ,1.0f };
		cb_ps_Light.data.ambientLightStrength = 0.0;

		// Model
		
		//if (!gameObject.Initialize("Data\\Object\\nanosuit\\nanosuit.obj",device.Get(), deviceContext.Get(), cb_vs_VertexShader)) return false;
		if (!gameObject.Initialize("Data\\Object\\Simple\\plane.fbx", device.Get(), deviceContext.Get(), cb_vs_VertexShader)) return false;
		if (!light.Initialize(device.Get(), deviceContext.Get(), cb_vs_VertexShader))
			return false;
		float nearZ = 0.1f;
		float farZ = 10000.0f;
		camera.SetPosition(0.0f, 0.0f, -2.0f);
		camera.SetProjectionValues(90.0f, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), nearZ, farZ);
	}
	catch (COMException& exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}
	return true;
}