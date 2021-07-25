#pragma once
#include "Model.hpp"

class GameObject
{
public:
	const XMVECTOR& GetPositionVector() const noexcept;
	const XMFLOAT3& GetPositionFloat3() const noexcept;
	const XMVECTOR& GetRotationVector() const noexcept;
	const XMFLOAT3& GetRotationFloat3() const noexcept;

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

protected:
	virtual void UpdateMatrix();

	XMVECTOR posVector;
	XMVECTOR rotVector;
	XMFLOAT3 pos;
	XMFLOAT3 rot;
};