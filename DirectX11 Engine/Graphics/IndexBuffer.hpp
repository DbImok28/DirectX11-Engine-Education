#pragma once
#include "../Base.hpp"

class IndexBuffer
{
private:
	com_ptr<ID3D11Buffer> buffer;
	UINT bufferSize = 0;
public:
	IndexBuffer(){}
	IndexBuffer(const IndexBuffer& vb);
	IndexBuffer& operator=(const IndexBuffer& vb);
	IndexBuffer(IndexBuffer&& vb) noexcept;
	IndexBuffer& operator=(IndexBuffer&& vb) noexcept;
	ID3D11Buffer* Get() const noexcept;
	ID3D11Buffer* const* GetAddressOf() const noexcept;
	UINT IndexCount() const noexcept;
	HRESULT Initialize(ID3D11Device* device, DWORD* data, UINT numVertices);
};