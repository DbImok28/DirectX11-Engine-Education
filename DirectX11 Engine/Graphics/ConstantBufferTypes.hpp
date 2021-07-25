#pragma once
#include <DirectXMath.h>

struct alignas(16) CB_VS_VertexShader
{
	DirectX::XMMATRIX wvpMatrix;
	DirectX::XMMATRIX worldMatrix;
};

/*struct alignas(16) CB_PS_PixelShader
{
	float alpha = 1.0f;
};*/
struct alignas(16) CB_PS_Light
{
	DirectX::XMFLOAT3 ambientLightColor;
	float ambientLightStrength = 0.0f;

	DirectX::XMFLOAT3 dynamicLightColor;
	float dynamicLightStrength = 0.0f;
	DirectX::XMFLOAT3 dynamicLightPosition;
	float dynamicLightAttenuation_a = 1.0f;
	float dynamicLightAttenuation_b = 0.1f;
	float dynamicLightAttenuation_c = 0.1f;
};