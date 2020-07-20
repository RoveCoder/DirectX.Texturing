struct Material
{
	float4 mDiffuse;
	float4 mAmbient;
	float4 mSpecular;
};

cbuffer WorldBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;

	matrix TextureTransform;
	Material mMaterial;
}

struct VertexInput
{
	float3 Position : POSITION;
	float2 Texture : TEXCOORD0;
};

struct PixelInput
{
	float3 Position : POSITION;
	float4 PositionH : SV_POSITION;
	float2 Texture : TEXCOORD0;
};

SamplerState SamplerAnisotropic : register(s0);

Texture2D TextureDiffuse : register(t0);