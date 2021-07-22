#include "Camera.hpp"

using namespace DirectX;

Camera::Camera() : 
	pos(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
	posVector(DirectX::XMLoadFloat3(&pos)),
	rot(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
	rotVector(DirectX::XMLoadFloat3(&rot))
{
	UpdateViewMatrix();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 360.0f) * DirectX::XM_2PI;
	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

const DirectX::XMMATRIX& Camera::GetViewMatrix() const noexcept
{
	return viewMatrix;
}

const DirectX::XMMATRIX& Camera::GetProjectionMatrix() const noexcept
{
	return projectionMatrix;
}

const DirectX::XMVECTOR& Camera::GetPositionVector() const noexcept
{
	return posVector;
}

const DirectX::XMFLOAT3& Camera::GetPositionFloat3() const noexcept
{
	return pos;
}

const DirectX::XMVECTOR& Camera::GetRotationVector() const noexcept
{
	return rotVector;
}

const DirectX::XMFLOAT3& Camera::GetRotationFloat3() const noexcept
{
	return rot;
}

void Camera::SetPosition(const DirectX::XMVECTOR& pos) noexcept
{
	posVector = pos;
	DirectX::XMStoreFloat3(&this->pos, pos);
	UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z) noexcept
{
	pos = DirectX::XMFLOAT3(x, y, z);
	posVector = DirectX::XMLoadFloat3(&pos);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(const DirectX::XMVECTOR& pos) noexcept
{
	posVector += pos;
	DirectX::XMStoreFloat3(&this->pos, posVector);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z) noexcept
{
	pos.x += x;
	pos.y += y;
	pos.z += z;
	posVector = DirectX::XMLoadFloat3(&pos);
	UpdateViewMatrix();
}

void Camera::SetRotation(const DirectX::XMVECTOR& rot) noexcept
{
	rotVector = rot;
	DirectX::XMStoreFloat3(&this->rot, rot);
	UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z) noexcept
{
	rot = DirectX::XMFLOAT3(x, y, z);
	rotVector = DirectX::XMLoadFloat3(&rot);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(const DirectX::XMVECTOR& rot) noexcept
{
	rotVector += rot;
	DirectX::XMStoreFloat3(&this->rot, rotVector);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z) noexcept
{
	rot.x += x;
	rot.y += y;
	rot.z += z;
	rotVector = DirectX::XMLoadFloat3(&rot);
	UpdateViewMatrix();
}

void Camera::SetLookAtPos(DirectX::XMFLOAT3 lookAtPos) noexcept
{
	if (lookAtPos.x == pos.x && lookAtPos.y == pos.y && lookAtPos.z == pos.z)
		return;
	lookAtPos.x = pos.x - lookAtPos.x;
	lookAtPos.y = pos.y - lookAtPos.y;
	lookAtPos.z = pos.z - lookAtPos.z;

	float pitch = 0.0f;
	if (lookAtPos.y != 0.0f)
	{
		const float distance = sqrtf(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
		pitch = atanf(lookAtPos.y / distance);
	}
	float yaw = 0.0f;
	if (lookAtPos.x != 0.0f)
	{
		yaw = atanf(lookAtPos.x / lookAtPos.z);
	}
	if (lookAtPos.z > 0.0f)
		yaw += DirectX::XM_PI;
	SetRotation(pitch, yaw, 0.0f);
}

void Camera::UpdateViewMatrix() noexcept
{
	DirectX::XMMATRIX camRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	DirectX::XMVECTOR camTarget = DirectX::XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, camRotationMatrix);
	camTarget += posVector;
	DirectX::XMVECTOR upDir = DirectX::XMVector3TransformCoord(DEFAULT_UP_VECTOR, camRotationMatrix);
	viewMatrix = DirectX::XMMatrixLookAtLH(posVector, camTarget, upDir);
}