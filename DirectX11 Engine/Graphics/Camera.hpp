#pragma once
#include "../Base.hpp"
#include <DirectXMath.h>

class Camera
{
public:
	Camera();
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	const DirectX::XMMATRIX& GetViewMatrix() const noexcept;
	const DirectX::XMMATRIX& GetProjectionMatrix() const noexcept;

	const DirectX::XMVECTOR& GetPositionVector() const noexcept;
	const DirectX::XMFLOAT3& GetPositionFloat3() const noexcept;
	const DirectX::XMVECTOR& GetRotationVector() const noexcept;
	const DirectX::XMFLOAT3& GetRotationFloat3() const noexcept;

	void SetPosition(const DirectX::XMVECTOR& pos) noexcept;
	void SetPosition(float x, float y, float z) noexcept;
	void AdjustPosition(const DirectX::XMVECTOR& pos) noexcept;
	void AdjustPosition(float x, float y, float z) noexcept;

	void SetRotation(const DirectX::XMVECTOR& rot) noexcept;
	void SetRotation(float x, float y, float z) noexcept;
	void AdjustRotation(const DirectX::XMVECTOR& rot) noexcept;
	void AdjustRotation(float x, float y, float z) noexcept;
	void SetLookAtPos(DirectX::XMFLOAT3) noexcept;
private:
	void UpdateViewMatrix() noexcept;
	DirectX::XMVECTOR posVector;
	DirectX::XMVECTOR rotVector;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 rot;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projectionMatrix;

	const DirectX::XMVECTOR DEFAULT_FORWARD_VECTOR	= DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);	// z // ось вверх!!!
	const DirectX::XMVECTOR DEFAULT_UP_VECTOR		= DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);	// y // ось вперед!!!
};