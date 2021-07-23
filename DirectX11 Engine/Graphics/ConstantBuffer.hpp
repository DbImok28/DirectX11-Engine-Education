#pragma once
#include"../Base.hpp"
#include"../ErrorLogger.hpp"
#include "ConstantBufferTypes.hpp"

template<class T>
class ConstantBuffer
{
private:
	ConstantBuffer(const ConstantBuffer&);
private:
	com_ptr<ID3D11Buffer> buffer;
	ID3D11DeviceContext* deviceContext = nullptr;
public:
	ConstantBuffer():data(){}
	ConstantBuffer(T data) : data(data) {}
	T data;

	ID3D11Buffer* Get() const noexcept
	{
		return buffer.Get();
	}
	ID3D11Buffer* const* GetAddressOf() const noexcept
	{
		return buffer.GetAddressOf();
	}
	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		if (buffer.Get() != nullptr)
			buffer.Reset();

		this->deviceContext = deviceContext;

		D3D11_BUFFER_DESC desc;
		//ZeroMemory(&desc, sizeof(desc));

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
		desc.StructureByteStride = 0;

		return device->CreateBuffer(&desc, 0, buffer.GetAddressOf());
	}
	bool ApplyChanges()
	{
		D3D11_MAPPED_SUBRESOURCE mappedResorce;
		HRESULT hr = deviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResorce);
		if (FAILED(hr))
		{
			ErrorLogger::Log(hr, "Failed to map constant buffer.");
			return false;
		}
		CopyMemory(mappedResorce.pData, &data, sizeof(T));
		deviceContext->Unmap(buffer.Get(), 0);
		return true;
	}
};

