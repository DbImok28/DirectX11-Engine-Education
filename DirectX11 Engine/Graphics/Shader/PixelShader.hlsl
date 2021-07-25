/*cbuffer alphaBuffer : register(b0)
{
	float alpha;
};*/
cbuffer lightBuffer : register(b0)
{
	float3 ambientLightColor;
	float ambientLightStrength;
};
struct PS_INPUT
{
	float4 inPos : SV_POSITION;
	float2 inTexCoord : TEXCOORD;
};
Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState: SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	float3 samplerColor = objTexture.Sample(objSamplerState, input.inTexCoord);
	float3 ambientLight = ambientLightColor * ambientLightStrength;
    float3 finalColor = samplerColor * ambientLight;
	
    return float4(finalColor, 1.0f);
}