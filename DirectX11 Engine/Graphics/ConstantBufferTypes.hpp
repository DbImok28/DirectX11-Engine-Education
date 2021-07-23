#pragma once
#include <DirectXMath.h>

struct alignas(16) CB_VS_VertexShader
{
	DirectX::XMMATRIX mat;
};

struct alignas(16) CB_PS_PixelShader
{
	float alpha = 1.0f;
};