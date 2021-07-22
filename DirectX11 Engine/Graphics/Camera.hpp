#pragma once
#include "../Base.hpp"
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
	Camera();
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	const XMMATRIX& GetViewMatrix() const noexcept;
	const XMMATRIX& GetProjectionMatrix() const noexcept;

	const XMVECTOR& GetPositionVector() const noexcept;
	const XMFLOAT3& GetPositionFloat3() const noexcept;
	const XMVECTOR& GetRotationVector() const noexcept;
	const XMFLOAT3& GetRotationFloat3() const noexcept;

	const XMVECTOR& GetForwardVector() const noexcept;
	const XMVECTOR& GetBackwardVector() const noexcept;
	const XMVECTOR& GetRightVector() const noexcept;
	const XMVECTOR& GetLeftVector() const noexcept;

	void SetPosition(const XMVECTOR& pos) noexcept;
	void SetPosition(float x, float y, float z) noexcept;
	void AdjustPosition(const XMVECTOR& pos) noexcept;
	void AdjustPosition(float x, float y, float z) noexcept;

	void SetRotation(const XMVECTOR& rot) noexcept;
	void SetRotation(float x, float y, float z) noexcept;
	void AdjustRotation(const XMVECTOR& rot) noexcept;
	void AdjustRotation(float x, float y, float z) noexcept;
	void SetLookAtPos(XMFLOAT3) noexcept;
private:
	void UpdateViewMatrix() noexcept;
	XMVECTOR posVector;
	XMVECTOR rotVector;
	XMFLOAT3 pos;
	XMFLOAT3 rot;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

	const XMVECTOR DEFAULT_FORWARD_VECTOR	= XMVectorSet( 0.0f,  0.0f, 1.0f, 0.0f);	// z // ось вверх!!!
	const XMVECTOR DEFAULT_UP_VECTOR		= XMVectorSet( 0.0f,  1.0f, 0.0f, 0.0f);	// y // ось вперед!!!
	const XMVECTOR DEFAULT_BACKWARD_VECTOR	= XMVectorSet( 0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR		= XMVectorSet( 1.0f,  0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR		= XMVectorSet(-1.0f,  0.0f, 0.0f, 0.0f);

	XMVECTOR vec_forward;
	XMVECTOR vec_backward;
	XMVECTOR vec_right;
	XMVECTOR vec_left;
};