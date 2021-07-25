#pragma once
#include "GameObject2D.hpp"

class Camera2D : public GameObject2D
{
public:
	Camera2D();
	void SetProjectionValues(float width, float height, float nearZ, float farZ);
	const XMMATRIX& GatOrthoMatrix() const;
	const XMMATRIX& GatWorldMatrix() const;
private:
	void UpdateMatrix() override;

	XMMATRIX orthoMatrix;
	XMMATRIX worldMatrix;
};