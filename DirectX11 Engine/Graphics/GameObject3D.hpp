#pragma once
#include "GameObject.hpp"

class GameObject3D : public GameObject
{
public:
	const XMVECTOR& GetForwardVector(bool omitY = false) const noexcept;
	const XMVECTOR& GetBackwardVector(bool omitY = false) const noexcept;
	const XMVECTOR& GetRightVector(bool omitY = false) const noexcept;
	const XMVECTOR& GetLeftVector(bool omitY = false) const noexcept;
	void SetLookAtPos(XMFLOAT3 lookAtPos) noexcept;
protected:
	void UpdateMatrix() override;
	void UpdateDirectionVectors();

	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR vec_forward;
	XMVECTOR vec_backward;
	XMVECTOR vec_right;
	XMVECTOR vec_left;

	XMVECTOR vec_forward_noY;
	XMVECTOR vec_backward_noY;
	XMVECTOR vec_right_noY;
	XMVECTOR vec_left_noY;
};