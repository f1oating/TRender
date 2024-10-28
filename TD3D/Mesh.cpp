#include "TMesh.h"

#include "TD3D.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <list>

TMesh::TMesh(TVertex* vertices, size_t vertexCount, unsigned short* indices, size_t indexCount) {
    device = TD3D::GetTD3D()->GetDevice();
    context = TD3D::GetTD3D()->GetDeviceContext();

    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = vertexCount * sizeof(TVertex);
    bd.StructureByteStride = sizeof(TVertex);

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;
    device->CreateBuffer(&bd, &sd, &vertexBuffer);

    D3D11_BUFFER_DESC ibd = {};
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.CPUAccessFlags = 0u;
    ibd.MiscFlags = 0u;
    ibd.ByteWidth = indexCount * sizeof(unsigned short);
    ibd.StructureByteStride = sizeof(unsigned short);
    D3D11_SUBRESOURCE_DATA isd = {};
    isd.pSysMem = indices;
    device->CreateBuffer(&ibd, &isd, &indexBuffer);

    indicesToRender = indexCount;
}

TMesh::~TMesh() {
    RELEASE_COM(vertexBuffer)
    RELEASE_COM(indexBuffer)
}

void TMesh::Render() {
    HRESULT hr;

    const UINT stride = sizeof(TVertex);
    const UINT offset = 0u;

    context->IASetVertexBuffers(0, 1u, &vertexBuffer, &stride, &offset);
    context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0u);

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    context->DrawIndexed(indicesToRender, 0, 0);
}
