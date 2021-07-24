#pragma once
#include "Vertex.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "ConstantBuffer.hpp"

using namespace DirectX;

class Model
{
public:
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_VertexShader>& cb_vs_VertexShader);
	void SetTexture(ID3D11ShaderResourceView* texture);
	void Draw(const XMMATRIX& viewProjectionMatrix);

	const XMVECTOR& GetPositionVector() const noexcept;
	const XMFLOAT3& GetPositionFloat3() const noexcept;
	const XMVECTOR& GetRotationVector() const noexcept;
	const XMFLOAT3& GetRotationFloat3() const noexcept;

	const XMVECTOR& GetForwardVector() const noexcept;
	const XMVECTOR& GetBackwardVector() const noexcept;
	const XMVECTOR& GetRightVector() const noexcept;
	const XMVECTOR& GetLeftVector() const noexcept;

	void SetPosition(const XMVECTOR& pos) noexcept;
	void SetPosition(const XMFLOAT3& pos) noexcept;
	void SetPosition(float x, float y, float z) noexcept;
	void AdjustPosition(const XMVECTOR& pos) noexcept;
	void AdjustPosition(const XMFLOAT3& pos) noexcept;
	void AdjustPosition(float x, float y, float z) noexcept;

	void SetRotation(const XMVECTOR& rot) noexcept;
	void SetRotation(const XMFLOAT3& rot) noexcept;
	void SetRotation(float x, float y, float z) noexcept;
	void AdjustRotation(const XMVECTOR& rot) noexcept;
	void AdjustRotation(const XMFLOAT3& rot) noexcept;
	void AdjustRotation(float x, float y, float z) noexcept;
	void SetLookAtPos(XMFLOAT3) noexcept;
private:
	void UpdateWorldMatrix();
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ConstantBuffer<CB_VS_VertexShader>* cb_vs_VertexShader;
	ID3D11ShaderResourceView* texture = nullptr;

	VertexBuffer<Vertex> vertexBuffer;
	IndexBuffer indexBuffer;

	XMMATRIX worldMatrix = XMMatrixIdentity();

	XMVECTOR posVector;
	XMVECTOR rotVector;
	XMFLOAT3 pos;
	XMFLOAT3 rot;

	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);	
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR vec_forward;
	XMVECTOR vec_backward;
	XMVECTOR vec_right;
	XMVECTOR vec_left;
};