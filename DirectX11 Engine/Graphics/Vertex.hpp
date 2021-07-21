#pragma once
#include <DirectXMath.h>

struct Vertex
{
	Vertex() : pos(0.f, 0.f, 0.0f), color(0.0f, 0.0f, 0.0f) {}
	Vertex(float x, float y, float z) : pos(x, y, z), color(0.0f, 0.0f, 0.0f) {}
	Vertex(float x, float y, float z, float r, float g, float b) : pos(x, y, z), color(r, g, b) {}
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 color;
};