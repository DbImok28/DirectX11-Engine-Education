#include "Model.hpp"

bool Model::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_VertexShader>& cb_vs_VertexShader)
{
	this->device = device;
	this->deviceContext = deviceContext;
	this->texture = texture;
	this->cb_vs_VertexShader = &cb_vs_VertexShader;

	// Vertices
	Vertex v[]
	{
		{-0.5f, -0.5f, -0.5f, 0.0f, 1.0f}, // front
		{-0.5f,  0.5f, -0.5f, 0.0f, 0.0f},
		{ 0.5f,  0.5f, -0.5f, 1.0f, 0.0f},
		{ 0.5f, -0.5f, -0.5f, 1.0f, 1.0f},

		{-0.5f, -0.5f, 0.5f, 0.0f, 1.0f}, // back
		{-0.5f,  0.5f, 0.5f, 0.0f, 0.0f},
		{ 0.5f,  0.5f, 0.5f, 1.0f, 0.0f},
		{ 0.5f, -0.5f, 0.5f, 1.0f, 1.0f},
	};
	try
	{
		HRESULT hr = vertexBuffer.Initialize(device, v, ARRAYSIZE(v));
		COM_ERROR_IF(hr, "Failed to initialize vertex buffer.");

		// Indices
		DWORD indices[] =
		{
			0, 1, 2, 0, 2, 3, // front
			4, 7, 6, 4, 6, 5, // back
			3, 2, 6, 3, 6, 7, // right
			4, 5, 1, 4, 1, 0, // left
			1, 5, 6, 1, 6, 2, // top
			0, 3, 7, 0, 7, 4, // bottom
		};
		hr = indexBuffer.Initialize(device, indices, ARRAYSIZE(indices));
		COM_ERROR_IF(hr, "Failed to initialize index buffer.");
	}
	catch (COMException exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}
	SetPosition(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f, 0.0f, 0.0f);
	//UpdateWorldMatrix();
	return true;
}

void Model::SetTexture(ID3D11ShaderResourceView* texture)
{
	this->texture = texture;
}

void Model::Draw(const XMMATRIX& viewProjectionMatrix)
{
	cb_vs_VertexShader->data.mat = worldMatrix * viewProjectionMatrix;
	cb_vs_VertexShader->data.mat = XMMatrixTranspose(cb_vs_VertexShader->data.mat);
	//if (!cb_vs_VertexShader->ApplyChanges()) return;
	cb_vs_VertexShader->ApplyChanges();
	deviceContext->VSSetConstantBuffers(0, 1, cb_vs_VertexShader->GetAddressOf());

	/*cb_ps_PixelShader.data.alpha = 0.5f;
	if (!cb_ps_PixelShader.ApplyChanges()) return;
	deviceContext->PSSetConstantBuffers(0, 1, cb_ps_PixelShader.GetAddressOf());*/

	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
	deviceContext->DrawIndexed(indexBuffer.BufferSize(), 0, 0);
}

void Model::UpdateWorldMatrix()
{
	worldMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * XMMatrixTranslation(pos.x, pos.y, pos.z);

	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, 0.0f);
	vec_forward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	vec_backward = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	vec_right = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
	vec_left = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrix);
}

const XMVECTOR& Model::GetPositionVector() const noexcept
{
	return posVector;
}

const XMFLOAT3& Model::GetPositionFloat3() const noexcept
{
	return pos;
}

const XMVECTOR& Model::GetRotationVector() const noexcept
{
	return rotVector;
}

const XMFLOAT3& Model::GetRotationFloat3() const noexcept
{
	return rot;
}

const XMVECTOR& Model::GetForwardVector() const noexcept
{
	return vec_forward;
}

const XMVECTOR& Model::GetBackwardVector() const noexcept
{
	return vec_backward;
}

const XMVECTOR& Model::GetRightVector() const noexcept
{
	return vec_right;
}

const XMVECTOR& Model::GetLeftVector() const noexcept
{
	return vec_left;
}

void Model::SetPosition(const XMVECTOR& pos) noexcept
{
	posVector = pos;
	XMStoreFloat3(&this->pos, pos);
	UpdateWorldMatrix();
}

void Model::SetPosition(const XMFLOAT3& pos) noexcept
{
	this->pos = pos;
	posVector = XMLoadFloat3(&this->pos);
	UpdateWorldMatrix();
}

void Model::SetPosition(float x, float y, float z) noexcept
{
	pos = XMFLOAT3(x, y, z);
	posVector = XMLoadFloat3(&pos);
	UpdateWorldMatrix();
}

void Model::AdjustPosition(const XMVECTOR& pos) noexcept
{
	posVector += pos;
	XMStoreFloat3(&this->pos, posVector);
	UpdateWorldMatrix();
}

void Model::AdjustPosition(const XMFLOAT3& pos) noexcept
{
	this->pos.x += pos.x;
	this->pos.y += pos.y;
	this->pos.z += pos.z;
	posVector = XMLoadFloat3(&this->pos);
	UpdateWorldMatrix();
}

void Model::AdjustPosition(float x, float y, float z) noexcept
{
	pos.x += x;
	pos.y += y;
	pos.z += z;
	posVector = XMLoadFloat3(&pos);
	UpdateWorldMatrix();
}

void Model::SetRotation(const XMVECTOR& rot) noexcept
{
	rotVector = rot;
	XMStoreFloat3(&this->rot, rot);
	UpdateWorldMatrix();
}

void Model::SetRotation(const XMFLOAT3& rot) noexcept
{
	this->rot = rot;
	rotVector = XMLoadFloat3(&this->rot);
	UpdateWorldMatrix();
}

void Model::SetRotation(float x, float y, float z) noexcept
{
	rot = XMFLOAT3(x, y, z);
	rotVector = XMLoadFloat3(&rot);
	UpdateWorldMatrix();
}

void Model::AdjustRotation(const XMVECTOR& rot) noexcept
{
	rotVector += rot;
	XMStoreFloat3(&this->rot, rotVector);
	UpdateWorldMatrix();
}

void Model::AdjustRotation(const XMFLOAT3& rot) noexcept
{
	this->rot.x += rot.x;
	this->rot.y += rot.y;
	this->rot.z += rot.z;
	rotVector = XMLoadFloat3(&this->rot);
	UpdateWorldMatrix();
}

void Model::AdjustRotation(float x, float y, float z) noexcept
{
	rot.x += x;
	rot.y += y;
	rot.z += z;
	rotVector = XMLoadFloat3(&rot);
	UpdateWorldMatrix();
}

void Model::SetLookAtPos(XMFLOAT3 lookAtPos) noexcept
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
		yaw += XM_PI;
	SetRotation(pitch, yaw, 0.0f);
}