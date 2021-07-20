#pragma once
#include "../Base.hpp"
#include "../ErrorLogger.hpp"
#pragma comment(lib, "D3DCompiler.lib")
#include <d3dcompiler.h>

class VertexShader
{
public:
	bool Initalize(com_ptr<ID3D11Device>& device, std::wstring shaderpath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements);
	ID3D11VertexShader* GetShader() const noexcept;
	ID3D10Blob* GetBuffer() const noexcept;
	ID3D11InputLayout* GatInputLoyout() const noexcept;
private:
	com_ptr<ID3D11VertexShader> shader = nullptr;
	com_ptr<ID3D10Blob> shader_buffer = nullptr;
	com_ptr<ID3D11InputLayout> inputLoyout = nullptr;
};

class PixelShader
{
public:
	bool Initalize(com_ptr<ID3D11Device>& device, std::wstring shaderpath);
	ID3D11PixelShader* GetShader() const noexcept;
	ID3D10Blob* GetBuffer() const noexcept;
private:
	com_ptr<ID3D11PixelShader> shader = nullptr;
	com_ptr<ID3D10Blob> shader_buffer = nullptr;
};