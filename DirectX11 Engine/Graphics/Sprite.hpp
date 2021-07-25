#pragma once
#include "GameObject2D.hpp"
#include "Vertex.hpp"
#include "Texture.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "ConstantBuffer.hpp"

class Sprite : public GameObject2D
{
public:
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContextfloat, float width, float height, std::string spritePath, ConstantBuffer<CB_VS_VertexShader_2D>& cb_vs_VertexShader_2d);
	void Draw(XMMATRIX orthoMatrix);
	float GetWidth();
	float GetHeight();
private:
	void UpdateMatrix() override;

	ConstantBuffer<CB_VS_VertexShader_2D>* cb_vs_VertexShader_2d = nullptr;
	XMMATRIX worldMatrix = XMMatrixIdentity();
	std::unique_ptr<Texture> texture;
	ID3D11DeviceContext* deviceContext = nullptr;

	IndexBuffer indices;
	VertexBuffer<Vertex2D> vertices;
};