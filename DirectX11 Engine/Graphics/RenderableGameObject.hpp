#pragma once
#include "GameObject.hpp"

class RenderableGameObject : public GameObject
{
public:
	bool Initialize(std::string path, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_VertexShader>& cb_vs_VertexShader);
	void Draw(const XMMATRIX& viewProjectionMatrix);
private:
	void UpdateMatrix() override;

	Model model;
	XMMATRIX worldMatrix = XMMatrixIdentity();
};