#include "TDXMesh.h"
#include "TDXRenderDevice.h"

TDXMesh::TDXMesh() :
    m_NumIndices(0),
    m_NumVertices(0),
    m_RotationX(0.0f),
    m_RotationY(0.0f),
    m_RotationZ(0.0f),
    m_Pos({ 0.0f, 0.0f, 0.0f })
{
}

TDXMesh::TDXMesh(TVertexColor* vertices, unsigned short numVertices, TRenderDevice* tRenderDevice)
{
    TDXMesh();

    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = numVertices * sizeof(TVertexColor);
    bd.StructureByteStride = sizeof(TVertexColor);

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;
    ((TDXRenderDevice*)tRenderDevice)->m_pDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer);

    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.ByteWidth = sizeof(TConstantBuffer);
    cbDesc.Usage = D3D11_USAGE_DEFAULT;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = 0;
    ((TDXRenderDevice*)tRenderDevice)->m_pDevice->CreateBuffer(&cbDesc, nullptr, &m_pConstantBuffer);

    m_NumIndices = 0;
    m_NumVertices = numVertices;
}

TDXMesh::TDXMesh(TVertexColor* vertices, unsigned short numVertices, unsigned short* indices, unsigned short numIndices, TRenderDevice* tRenderDevice)
{
    TDXMesh();

    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = numVertices * sizeof(TVertexColor);
    bd.StructureByteStride = sizeof(TVertexColor);

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;
    ((TDXRenderDevice*)tRenderDevice)->m_pDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer);

    D3D11_BUFFER_DESC ibd = {};
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.CPUAccessFlags = 0u;
    ibd.MiscFlags = 0u;
    ibd.ByteWidth = numIndices * sizeof(unsigned short);
    ibd.StructureByteStride = sizeof(unsigned short);
    D3D11_SUBRESOURCE_DATA isd = {};
    isd.pSysMem = indices;
    ((TDXRenderDevice*)tRenderDevice)->m_pDevice->CreateBuffer(&ibd, &isd, &m_pIndexBuffer);

    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.ByteWidth = sizeof(TConstantBuffer);
    cbDesc.Usage = D3D11_USAGE_DEFAULT;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = 0;
    ((TDXRenderDevice*)tRenderDevice)->m_pDevice->CreateBuffer(&cbDesc, nullptr, &m_pConstantBuffer);

    m_NumIndices = numIndices;
    m_NumVertices = numVertices;
}

void TDXMesh::SetPosition(float x, float y, float z)
{
    m_Pos.x = x;
    m_Pos.y = y;
    m_Pos.z = z;
}

void TDXMesh::SetRotationX(float angle)
{
    m_RotationX = angle;
}

void TDXMesh::SetRotationY(float angle)
{
    m_RotationY = angle;
}

void TDXMesh::SetRotationZ(float angle)
{
    m_RotationZ = angle;
}

void TDXMesh::Render(TRenderDevice* tRenderDevice)
{
    DirectX::XMMATRIX modelMatrix =
        DirectX::XMMatrixRotationZ(m_RotationZ) *
        DirectX::XMMatrixRotationY(m_RotationY) *
        DirectX::XMMatrixRotationX(m_RotationX) *
        DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);

    TConstantBuffer cb;
    cb.mvpMatrix = DirectX::XMMatrixTranspose(modelMatrix * ((TDXRenderDevice*)tRenderDevice)->m_ViewMatrix * ((TDXRenderDevice*)tRenderDevice)->m_ProjMatrix);

    ((TDXRenderDevice*)tRenderDevice)->m_pDeviceContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);

    ((TDXRenderDevice*)tRenderDevice)->m_pDeviceContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());

    const UINT stride = sizeof(TVertexColor);
    const UINT offset = 0u;

    ((TDXRenderDevice*)tRenderDevice)->m_pDeviceContext->IASetVertexBuffers(0, 1u, m_pVertexBuffer.GetAddressOf(), &stride, &offset);

    if (m_NumIndices)
    {
        ((TDXRenderDevice*)tRenderDevice)->m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
        ((TDXRenderDevice*)tRenderDevice)->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        ((TDXRenderDevice*)tRenderDevice)->m_pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
    }
    else
    {
        ((TDXRenderDevice*)tRenderDevice)->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        ((TDXRenderDevice*)tRenderDevice)->m_pDeviceContext->Draw(m_NumVertices, 0);
    }
    
}
