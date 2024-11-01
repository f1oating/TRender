#include "TDXMesh.h"
#include "TDXRenderDevice.h"

void TDXMesh::CreateBuffers(TVertexColor* vertices, unsigned short numVertices, TRenderDevice* tRenderDevice)
{
    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = numVertices * sizeof(TVertexColor);
    bd.StructureByteStride = sizeof(TVertexColor);

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;
    ((TDXRenderDevice*) tRenderDevice)->m_pDevice->CreateBuffer(&bd, &sd, &m_VertexBuffer);

    numIndices = 0;
    this->numVertices = numVertices;
}

void TDXMesh::CreateBuffers(TVertexColor* vertices, unsigned short numVertices, unsigned short* indices, unsigned short numIndices, TRenderDevice* tRenderDevice)
{
    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = numVertices * sizeof(TVertexColor);
    bd.StructureByteStride = sizeof(TVertexColor);

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;
    ((TDXRenderDevice*)tRenderDevice)->m_pDevice->CreateBuffer(&bd, &sd, &m_VertexBuffer);

    D3D11_BUFFER_DESC ibd = {};
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.CPUAccessFlags = 0u;
    ibd.MiscFlags = 0u;
    ibd.ByteWidth = numIndices * sizeof(unsigned short);
    ibd.StructureByteStride = sizeof(unsigned short);
    D3D11_SUBRESOURCE_DATA isd = {};
    isd.pSysMem = indices;
    ((TDXRenderDevice*)tRenderDevice)->m_pDevice->CreateBuffer(&ibd, &isd, &m_IndexBuffer);

    this->numIndices = numIndices;
    this->numVertices = numVertices;
}

void TDXMesh::Render(TRenderDevice* tRenderDevice)
{
    const UINT stride = sizeof(TVertexColor);
    const UINT offset = 0u;

    ((TDXRenderDevice*)tRenderDevice)->m_pDeviceContext->IASetVertexBuffers(0, 1u, m_VertexBuffer.GetAddressOf(), &stride, &offset);

    if (numIndices)
    {
        ((TDXRenderDevice*)tRenderDevice)->m_pDeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
        ((TDXRenderDevice*)tRenderDevice)->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        ((TDXRenderDevice*)tRenderDevice)->m_pDeviceContext->DrawIndexed(numIndices, 0, 0);
    }
    else
    {
        ((TDXRenderDevice*)tRenderDevice)->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        ((TDXRenderDevice*)tRenderDevice)->m_pDeviceContext->Draw(numVertices, 0);
    }
    
}
