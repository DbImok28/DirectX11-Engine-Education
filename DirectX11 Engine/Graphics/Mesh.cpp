#include "Mesh.hpp"

Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<Vertex3D>& vertices, std::vector<DWORD>& indices, std::vector<Texture>& textures, const XMMATRIX& trasformMatrix)
	: deviceContext(deviceContext),
	textures(textures),
	trasformMatrix(trasformMatrix)
{
	HRESULT hr = vertexBuffer.Initialize(device, vertices.data(), vertices.size());
	COM_ERROR_IF(hr, "Failed to initialize vertex buffer for mesh.");

	hr = indexBuffer.Initialize(device, indices.data(), indices.size());
	COM_ERROR_IF(hr, "Failed to initialize index buffer for mesh.");
}

Mesh::Mesh(const Mesh& mesh) :
	deviceContext(mesh.deviceContext),
	indexBuffer(mesh.indexBuffer),
	vertexBuffer(mesh.vertexBuffer),
	textures(mesh.textures),
	trasformMatrix(mesh.trasformMatrix)
{

}

void Mesh::Draw()
{
	for (size_t i = 0; i < textures.size(); i++)
	{
		if (textures[i].GetType() == aiTextureType::aiTextureType_DIFFUSE)
		{
			deviceContext->PSSetShaderResources(0, 1, textures[i].GetTextureResourceViewAddress());
			break;
		}
	}

	UINT offset = 0;
	deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
	deviceContext->DrawIndexed(indexBuffer.IndexCount(), 0, 0);
}

const XMMATRIX& Mesh::GetTransformMatrix() const noexcept
{
	return trasformMatrix;
}
