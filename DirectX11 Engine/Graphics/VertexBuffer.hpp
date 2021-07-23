#pragma once
#include "../Base.hpp"

template<class T>
class VertexBuffer
{
private:
	VertexBuffer(const VertexBuffer<T>& vb);
private:
	com_ptr<ID3D11Buffer> buffer;
	std::unique_ptr<UINT> stride;
	UINT bufferSize = 0;
public:
	VertexBuffer(){}
	ID3D11Buffer* Get() const noexcept
	{
		return buffer.Get();
	}
	ID3D11Buffer* const* GetAddressOf() const noexcept
	{
		return buffer.GetAddressOf();
	}
	UINT BufferSize() const noexcept
	{
		return bufferSize;
	}
	const UINT Stride() const noexcept
	{
		return *stride.get();
	}
	const UINT* StridePtr() const noexcept
	{
		return stride.get();
	}
	HRESULT Initialize(ID3D11Device* device, T* data, UINT numVertices)
	{
		if (buffer.Get() != nullptr)
			buffer.Reset();

		bufferSize = numVertices;
		if(stide.get() == nullptr)
			stride = std::make_unique<UINT>(sizeof(T));

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

		vertexBufferDesc.ByteWidth = sizeof(T) * numVertices;
		vertexBufferDesc.StructureByteStride = sizeof(T);
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