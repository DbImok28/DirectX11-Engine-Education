#pragma once
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
#include "../Base.hpp"
#include "../Paths.hpp"
#include "../Timer.hpp"
#include "AdapterReader.hpp"
#include "Camera.hpp"
#include "Shaders.hpp"
#include "RenderableGameObject.hpp"
#include "Light.hpp"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

class Graphics
{
public:
	bool Initialize(HWND hWnd, int width, int height);
	void RenderFrame();

	Camera camera;
	RenderableGameObject gameObject;
	Light light;
private:
	bool InitializeDirectX(HWND hWnd);
	bool InitializeShader();
	bool InitializeScene();

	com_ptr<ID3D11Device> device;
	com_ptr<ID3D11DeviceContext> deviceContext;
	com_ptr<IDXGISwapChain> swapchain;
	com_ptr<ID3D11RenderTargetView> renderTargetView;

	VertexShader vertexShader;
	PixelShader pixelShader;
	PixelShader pixelShaderNoLight;

	ConstantBuffer<CB_VS_VertexShader> cb_vs_VertexShader;
	ConstantBuffer<CB_PS_Light> cb_ps_Light;


	com_ptr<ID3D11DepthStencilView> depthStencilView;
	com_ptr<ID3D11Texture2D> depthStencilBuffer;
	com_ptr<ID3D11DepthStencilState> depthStencilState;

	com_ptr<ID3D11RasterizerState> rasterizerState;
	/*com_ptr<ID3D11RasterizerState> rasterizerState_CullFront;*/
	com_ptr<ID3D11BlendState> blendState;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	com_ptr<ID3D11SamplerState> samplerState;
	com_ptr<ID3D11ShaderResourceView> myTexture;
	com_ptr<ID3D11ShaderResourceView> grassTexture;
	com_ptr<ID3D11ShaderResourceView> concreteTexture;

	int windowWidth = 0;
	int windowHeight = 0;

	Timer fpsTimer;
};