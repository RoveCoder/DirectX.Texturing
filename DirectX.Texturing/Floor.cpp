#include "Floor.h"
#include "GeometryGenerator.h"
#include "DDSTextureLoader.h"
#include "ShaderData.h"

Floor::Floor(Renderer* renderer) : m_Renderer(renderer)
{
    m_Material.mDiffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}

bool Floor::Load()
{
    Geometry::CreateGrid(10.0f, 10.0f, 2, 2, &m_MeshData);

    // Create vertex buffer
    D3D11_BUFFER_DESC vbd = {};
    vbd.Usage = D3D11_USAGE_DEFAULT;
    vbd.ByteWidth = (UINT)(sizeof(Vertex) * m_MeshData.vertices.size());
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA vInitData = {};
    vInitData.pSysMem = &m_MeshData.vertices[0];

    DX::ThrowIfFailed(m_Renderer->GetDevice()->CreateBuffer(&vbd, &vInitData, &m_VertexBuffer));

    // Create index buffer
    D3D11_BUFFER_DESC ibd = {};
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.ByteWidth = (UINT)(sizeof(UINT) * m_MeshData.indices.size());
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA iInitData = {};
    iInitData.pSysMem = &m_MeshData.indices[0];

    DX::ThrowIfFailed(m_Renderer->GetDevice()->CreateBuffer(&ibd, &iInitData, &m_IndexBuffer));

    // Constant buffer
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ConstantBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    DX::ThrowIfFailed(m_Renderer->GetDevice()->CreateBuffer(&bd, nullptr, &m_ConstantBuffer));

    // Load texture
    ID3D11Resource* resource = nullptr;
    DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(m_Renderer->GetDevice(), L"Textures\\stone_wall_diffuse.dds", &resource, &m_DiffuseTexture));
    resource->Release();

    return true;
}

void Floor::Render(Camera* camera)
{
    // Bind the vertex buffer
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    m_Renderer->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

    // Bind the index buffer
    m_Renderer->GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set topology
    m_Renderer->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Set buffer
    DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(0.0f, -1.0f, 0.0f);
    DirectX::XMMATRIX textureTransform = DirectX::XMMatrixIdentity();

    ConstantBuffer cb;
    cb.mWorld = DirectX::XMMatrixTranspose(world);
    cb.mView = DirectX::XMMatrixTranspose(camera->GetView());
    cb.mProjection = DirectX::XMMatrixTranspose(camera->GetProjection());
    cb.mTextureTransform = DirectX::XMMatrixTranspose(textureTransform);
    cb.mMaterial = m_Material;

    m_Renderer->GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
    m_Renderer->GetDeviceContext()->PSSetConstantBuffers(0, 1, &m_ConstantBuffer);
    m_Renderer->GetDeviceContext()->UpdateSubresource(m_ConstantBuffer, 0, nullptr, &cb, 0, 0);

    m_Renderer->GetDeviceContext()->PSSetShaderResources(0, 1, &m_DiffuseTexture);

    // Render geometry
    m_Renderer->GetDeviceContext()->DrawIndexed((UINT)m_MeshData.indices.size(), 0, 0);
}
