#include "IndexBuffer.hpp"

IndexBuffer::IndexBuffer(const IndexBuffer& vb) 
	: buffer(vb.buffer), bufferSize(vb.bufferSize) {}

IndexBuffer& IndexBuffer::operator=(const IndexBuffer& vb)
{
	buffer = vb.buffer;
	bufferSize = vb.bufferSize;
	return *this;
}

IndexBuffer::IndexBuffer(IndexBuffer&& vb) noexcept
	:buffer(std::move(vb.buffer)), bufferSize(std::move(vb.bufferSize))
{
	vb.buffer = nullptr;
	vb.bufferSize = 0;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& vb) noexcept
{
	buffer = std::move(vb.buffer);
	bufferSize = std::move(vb.bufferSize);
	return *this;
}

ID3D11Buffer* IndexBuffer::Get() const noexcept
{
	return buffer.Get();
}

ID3D11Buffer* const* IndexBuffer::GetAddressOf() const noexcept
{
	return buffer.GetAddressOf();
}

UINT IndexBuffer::IndexCount() const noexcept
{
	return bufferSize;
}

HRESULT IndexBuffer::Initialize(ID3D11Device* device, DWORD* data, UINT indexCount)
{
	if (buffer.Get() != nullptr)
		buffer.Reset();
	

	bufferSize = indexCount;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.ByteWidth = sizeof(DWORD) * indexCount;
	indexBufferDesc.StructureByteStride = sizeof(DWORD);
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	//ZeroMemory(&indexBufferData, sizeof(indexBufferData));
	indexBufferData.pSysMem = data;

	return device->CreateBuffer(&indexBufferDesc, &indexBufferData, buffer.GetAddressOf());
}