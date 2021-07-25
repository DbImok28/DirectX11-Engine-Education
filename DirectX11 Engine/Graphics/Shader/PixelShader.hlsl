/*cbuffer alphaBuffer : register(b0)
{
	float alpha;
};*/
cbuffer lightBuffer : register(b0)
{
	float3 ambientLightColor;
	float ambientLightStrength;
	
	float3 dynamicLightColor;
	float dynamicLightStrength;
	float3 dynamicLightPosition;
	
	float dynamicLightAttenuation_a = 1.0f;
	float dynamicLightAttenuation_b = 0.1f;
	float dynamicLightAttenuation_c = 0.1f;
};
struct PS_INPUT
{
	float4 inPos : SV_POSITION;
	float2 inTexCoord : TEXCOORD;
	float3 inNormal : NORMAL;
	float3 inWorldPos : WORLD_POSITION;
};
Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState: SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	float3 samplerColor = objTexture.Sample(objSamplerState, input.inTexCoord);
	float3 ambientLight = ambientLightColor * ambientLightStrength;
	float3 appliedLight = ambientLight;
	
	float3 vectorToLight = normalize(dynamicLightPosition - input.inWorldPos);
	float3 diffuseLightIntensity = max(dot(vectorToLight, input.inNormal), 0); 
	float distanceToLight = distance(dynamicLightPosition, input.inWorldPos);
	float attenuationFactor = (1 / (dynamicLightAttenuation_a + dynamicLightAttenuation_b * distanceToLight + dynamicLightAttenuation_c * distanceToLight * distanceToLight));
	diffuseLightIntensity *= attenuationFactor;
	float3 diffuseLight = diffuseLightIntensity * dynamicLightStrength * dynamicLightColor; 
	
	appliedLight += diffuseLight;   
	
	float3 finalColor = samplerColor * appliedLight;	
	return float4(finalColor, 1.0f);
}