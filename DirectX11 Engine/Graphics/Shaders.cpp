#include "Shaders.hpp"
#include <sstream>

bool VertexShader::Initalize(com_ptr<ID3D11Device>& device, std::wstring shaderpath)
{
    HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), shader_buffer.GetAddressOf());
    if (FAILED(hr))
    {
        std::wstringstream ss;
        ss << L"Failed to load shader: " << shaderpath;
        ErrorLogger::Log(hr, ss.str().c_str());
        return false;
    }

    hr = device->CreateVertexShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), NULL,shader.GetAddressOf());
    if (FAILED(hr))
    {
        std::wstringstream ss;
        ss << L"Failed to create vertex shader: " << shaderpath;
        ErrorLogger::Log(hr, ss.str().c_str());
        return false;
    }
    return true;
}

ID3D11VertexShader* VertexShader::GetShader()
{
    return shader.Get();
}

ID3D10Blob* VertexShader::GetBuffer()
{
    return shader_buffer.Get();
}
