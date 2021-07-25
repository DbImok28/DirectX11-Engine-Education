#include "GameObject.hpp"

const XMVECTOR& GameObject::GetPositionVector() const noexcept
{
	return posVector;
}

const XMFLOAT3& GameObject::GetPositionFloat3() const noexcept
{
	return pos;
}

const XMVECTOR& GameObject::GetRotationVector() const noexcept
{
	return rotVector;
}

const XMFLOAT3& GameObject::GetRotationFloat3() const noexcept
{
	return rot;
}

void GameObject::SetPosition(const XMVECTOR& pos) noexcept
{
	posVector = pos;
	XMStoreFloat3(&this->pos, pos);
	UpdateMatrix();
}

void GameObject::SetPosition(const XMFLOAT3& pos) noexcept
{
	this->pos = pos;
	posVector = XMLoadFloat3(&this->pos);
	UpdateMatrix();
}

void GameObject::SetPosition(float x, float y, float z) noexcept
{
	pos = XMFLOAT3(x, y, z);
	posVector = XMLoadFloat3(&pos);
	UpdateMatrix();
}

void GameObject::AdjustPosition(const XMVECTOR& pos) noexcept
{
	posVector += pos;
	XMStoreFloat3(&this->pos, posVector);
	UpdateMatrix();
}

void GameObject::AdjustPosition(const XMFLOAT3& pos) noexcept
{
	this->pos.x += pos.x;
	this->pos.y += pos.y;
	this->pos.z += pos.z;
	posVector = XMLoadFloat3(&this->pos);
	UpdateMatrix();
}

void GameObject::AdjustPosition(float x, float y, float z) noexcept
{
	pos.x += x;
	pos.y += y;
	pos.z += z;
	posVector = XMLoadFloat3(&pos);
	UpdateMatrix();
}

void GameObject::SetRotation(const XMVECTOR& rot) noexcept
{
	rotVector = rot;
	XMStoreFloat3(&this->rot, rot);
	UpdateMatrix();
}

void GameObject::SetRotation(const XMFLOAT3& rot) noexcept
{
	this->rot = rot;
	rotVector = XMLoadFloat3(&this->rot);
	UpdateMatrix();
}

void GameObject::SetRotation(float x, float y, float z) noexcept
{
	rot = XMFLOAT3(x, y, z);
	rotVector = XMLoadFloat3(&rot);
	UpdateMatrix();
}

void GameObject::AdjustRotation(const XMVECTOR& rot) noexcept
{
	rotVector += rot;
	XMStoreFloat3(&this->rot, rotVector);
	UpdateMatrix();
}

void GameObject::AdjustRotation(const XMFLOAT3& rot) noexcept
{
	this->rot.x += rot.x;
	this->rot.y += rot.y;
	this->rot.z += rot.z;
	rotVector = XMLoadFloat3(&this->rot);
	UpdateMatrix();
}

void GameObject::AdjustRotation(float x, float y, float z) noexcept
{
	rot.x += x;
	rot.y += y;
	rot.z += z;
	rotVector = XMLoadFloat3(&rot);
	UpdateMatrix();
}

void GameObject::UpdateMatrix()
{
	assert("no overridden" && 0);
}