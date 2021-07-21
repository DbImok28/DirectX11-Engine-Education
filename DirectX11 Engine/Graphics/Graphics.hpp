#pragma once
#include "AdapterReader.hpp"
#include "Shaders.hpp"
#include "Vertex.hpp"

class Graphics
{
public:
	bool Initialize(HWND hWnd, int width, int height);
	void RenderFrame();
private:
	bool InitializeDirectX(HWND hWnd, int width, int height);
	bool InitializeShader();
	bool InitializeScene();

	com_ptr<ID3D11Device> device;
	com_ptr<ID3D11DeviceContext> deviceContext;
	com_ptr<IDXGISwapChain> swapchain;
	com_ptr<ID3D11RenderTargetView> renderTargetView;

	VertexShader vertexShader;
	PixelShader pixelShader;

	com_ptr<ID3D11Buffer>vertexBuffer;
};