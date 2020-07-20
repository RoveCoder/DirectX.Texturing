#pragma once

#include "Renderer.h"
#include "Camera.h"
#include "Mesh.h"
#include "ShaderData.h"

class Water
{
public:
	Water(Renderer* renderer);

	bool Load();
	void Render(Camera* camera, double deltaTime);

private:
	Renderer* m_Renderer = nullptr;

	MeshData m_MeshData;
	Material m_Material;

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11Buffer* m_IndexBuffer = nullptr;

	ID3D11Buffer* m_ConstantBuffer = nullptr;

	ID3D11ShaderResourceView* m_DiffuseTexture = nullptr;

	DirectX::XMMATRIX m_TextureTransform;
};