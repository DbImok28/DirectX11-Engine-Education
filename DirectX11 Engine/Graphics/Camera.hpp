#pragma once
#include "GameObject.hpp"

using namespace DirectX;

class Camera : public GameObject
{
public:
	Camera();
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	const XMMATRIX& GetViewMatrix() const noexcept;
	const XMMATRIX& GetProjectionMatrix() const noexcept;

private:
	void UpdateMatrix() override;

	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
};