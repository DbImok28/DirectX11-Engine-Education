#include "RenderableGameObject.hpp"

bool RenderableGameObject::Initialize(std::string path, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_VertexShader>& cb_vs_VertexShader)
{
	if (!model.Initialize(path, device, deviceContext, cb_vs_VertexShader))
		return false;
	SetPosition(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f, 0.0f, 0.0f);
	//UpdateWorldMatrix();
	return true;
}

void RenderableGameObject::Draw(const XMMATRIX& viewProjectionMatrix)
{
	model.Draw(worldMatrix, viewProjectionMatrix);
}

void RenderableGameObject::UpdateMatrix()
{
	worldMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * XMMatrixTranslation(pos.x, pos.y, pos.z);

	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, 0.0f);
	vec_forward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	vec_backward = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	vec_right = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
	vec_left = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrix);
}