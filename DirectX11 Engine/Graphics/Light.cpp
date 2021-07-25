#include "Light.hpp"

bool Light::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_VertexShader>& cb_vs_VertexShader)
{
    if (!model.Initialize("Data/Object/Light.fbx", device, deviceContext, cb_vs_VertexShader))
        return false;
    SetPosition(0.0f, 0.0f, 0.0f);
    SetRotation(0.0f, 0.0f, 0.0f);
    UpdateMatrix();
    return true;
}