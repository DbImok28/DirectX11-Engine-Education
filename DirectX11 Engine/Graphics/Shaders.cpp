#include "Shaders.hpp"
#include <sstream>

bool VertexShader::Initalize(com_ptr<ID3D11Device>& device, std::wstring shaderpath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements)
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

    hr = device->CreateInputLayout(layoutDesc, numElements, shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), inputLoyout.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create InputLayout(vertex shader).");
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

ID3D11InputLayout* VertexShader::GatInputLoyout()
{
    return inputLoyout.Get();
}
