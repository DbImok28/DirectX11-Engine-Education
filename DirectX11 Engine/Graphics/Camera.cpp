#include "Camera.hpp"

Camera::Camera()
{
	pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	posVector = XMLoadFloat3(&pos);
	rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotVector = XMLoadFloat3(&rot);
	UpdateMatrix();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
	projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

const XMMATRIX& Camera::GetViewMatrix() const noexcept
{
	return viewMatrix;
}

const XMMATRIX& Camera::GetProjectionMatrix() const noexcept
{
	return projectionMatrix;
}

void Camera::UpdateMatrix()
{
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	XMVECTOR camTarget = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, camRotationMatrix);
	camTarget += posVector;
	XMVECTOR upDir = XMVector3TransformCoord(DEFAULT_UP_VECTOR, camRotationMatrix);
	viewMatrix = XMMatrixLookAtLH(posVector, camTarget, upDir);

	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, 0.0f);
	vec_forward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	vec_backward = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	vec_right = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
	vec_left = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrix);
}