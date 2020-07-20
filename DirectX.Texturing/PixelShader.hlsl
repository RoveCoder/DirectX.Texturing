#include "Header.hlsli"

float4 main(PixelInput input) : SV_TARGET
{
	float4 diffuse_texture = TextureDiffuse.Sample(SamplerAnisotropic, input.Texture);

	float4 finalColour = diffuse_texture * mMaterial.mDiffuse;
	finalColour.a = mMaterial.mDiffuse.a;

	return finalColour;
}