#pragma once
#include "../Base.hpp"
#include "../ErrorLogger.hpp"
#pragma comment(lib, "D3DCompiler.lib")
#include <d3dcompiler.h>

class VertexShader
{
public:
	bool Initalize(com_ptr<ID3D11Device>& device, std::wstring shaderpath);
	ID3D11VertexShader* GetShader();
	ID3D10Blob* GetBuffer();
private:
	com_ptr<ID3D11VertexShader> shader = nullptr;
	com_ptr<ID3D10Blob> shader_buffer = nullptr;
};