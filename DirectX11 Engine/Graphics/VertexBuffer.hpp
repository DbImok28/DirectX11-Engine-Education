#pragma once
#include "../Base.hpp"

template<class T>
class VertexBuffer
{
private:
	com_ptr<ID3D11Buffer> buffer;
	UINT stride = sizeof(T);
	UINT bufferSize = 0;
public:
	VertexBuffer(){}
	VertexBuffer(const VertexBuffer<T>& vb)
		:buffer(vb.buffer), bufferSize(vb.bufferSize), stride(vb.stride)
	{
	}
	VertexBuffer<T>& operator=(const VertexBuffer<T>& vb)
	{
		buffer = vb.buffer;
		bufferSize = vb.bufferSize;
		stride = vb.stride;
		return *this;
	}
	VertexBuffer(VertexBuffer&& vb) noexcept
		:buffer(std::move(vb.buffer)), bufferSize(std::move(vb.bufferSize)), stride(std::move(vb.stride))
	{
		vb.buffer = nullptr;
		vb.bufferSize = 0;
		vb.stride = nullptr;
	}
	VertexBuffer<T>& operator=(VertexBuffer<T>&& vb) noexcept
	{
		buffer = std::move(vb.buffer);
		bufferSize = std::move(vb.bufferSize);
		stride = std::move(vb.stride);
		return *this;
	}

	ID3D11Buffer* Get() const noexcept
	{
		return buffer.Get();
	}
	ID3D11Buffer* const* GetAddressOf() const noexcept
	{
		return buffer.GetAddressOf();
	}
	UINT VertexCount() const noexcept
	{
		return bufferSize;
	}
	const UINT Stride() const noexcept
	{
		return stride;
	}
	const UINT* StridePtr() const noexcept
	{
		return &stride;
	}
	HRESULT Initialize(ID3D11Device* device, T* data, UINT vertexCount)
	{
		if (buffer.Get() != nullptr)
			buffer.Reset();

		bufferSize = vertexCount;

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

		vertexBufferDesc.ByteWidth = stride * vertexCount;
		vertexBufferDesc.StructureByteStride = stride;
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
		vertexBufferData.pSysMem = data;

		return device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, buffer.GetAddressOf());
	}
};