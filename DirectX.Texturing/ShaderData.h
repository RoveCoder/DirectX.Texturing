#pragma once

#include <DirectXMath.h>

__declspec(align(16)) struct Material
{
    DirectX::XMFLOAT4 mDiffuse;
    DirectX::XMFLOAT4 mAmbient;
    DirectX::XMFLOAT4 mSpecular;
};

_declspec(align(16)) struct ConstantBuffer
{
    DirectX::XMMATRIX mWorld;
    DirectX::XMMATRIX mView;
    DirectX::XMMATRIX mProjection;

    DirectX::XMMATRIX mTextureTransform;
    Material mMaterial;
};