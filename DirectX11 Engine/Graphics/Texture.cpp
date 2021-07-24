#include "Texture.hpp"
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include "../ErrorLogger.hpp"
#include "../Base.hpp"

Texture::Texture(ID3D11Device* device, const Color& color, aiTextureType type)
{
    Initialize1x1ColorTexture(device, color, type);
}

Texture::Texture(ID3D11Device* device, const Color* color, UINT width, UINT height, aiTextureType type)
{
    InitializeColorTexture(device, color, width, height, type);
}

Texture::Texture(ID3D11Device* device, const std::string& filePath, aiTextureType type)
{
    this->type = type;

    if (Paths::GetFileExtension(filePath) == ".dds")
    {
        HRESULT hr = DirectX::CreateDDSTextureFromFile(device, StringHelper::StringToWide(filePath).c_str(), texture.GetAddressOf(), textureView.GetAddressOf());
        if (FAILED(hr))
        {
            Initialize1x1ColorTexture(device, Colors::UnloadedTextureColor, type);
        }
        return;
    }
    else
    {
        HRESULT hr = DirectX::CreateWICTextureFromFile(device, StringHelper::StringToWide(filePath).c_str(), texture.GetAddressOf(), textureView.GetAddressOf());
        if (FAILED(hr))
        {
            Initialize1x1ColorTexture(device, Colors::UnloadedTextureColor, type);
        }
    }
}

Texture::Texture(ID3D11Device* device, const uint8_t* pData, size_t size, aiTextureType type)
{
    this->type = type;
    HRESULT hr = DirectX::CreateWICTextureFromMemory(device, pData, size, texture.GetAddressOf(), textureView.GetAddressOf());
    COM_ERROR_IF(hr, "Failed to create texture from memory.");
}

aiTextureType Texture::GetType()
{
    return type;
}

ID3D11ShaderResourceView* Texture::GetTextureResourceView()
{
    return textureView.Get();
}

ID3D11ShaderResourceView** Texture::GetTextureResourceViewAddress()
{
    return textureView.GetAddressOf();
}

void Texture::Initialize1x1ColorTexture(ID3D11Device* device, const Color& color, aiTextureType type)
{
    InitializeColorTexture(device, &color, 1, 1, type);
}

void Texture::InitializeColorTexture(ID3D11Device* device, const Color* color, UINT width, UINT height, aiTextureType type)
{
    this->type = type;

    CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
    ID3D11Texture2D* p2DTexture = nullptr;
    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = color;
    initData.SysMemPitch = width * sizeof(Color);
    HRESULT hr = device->CreateTexture2D(&textureDesc, &initData, &p2DTexture);
    COM_ERROR_IF(hr, "Failed to initialize texture from color.");
    texture = static_cast<ID3D11Texture2D*>(p2DTexture);
    CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
    hr = device->CreateShaderResourceView(texture.Get(), &srvDesc, textureView.GetAddressOf());
    COM_ERROR_IF(hr, "Failed to create shader resource view");
}