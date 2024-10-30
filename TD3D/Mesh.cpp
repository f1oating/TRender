#include "TMesh.h"

#include "TD3D.h"
#include <cmath>
#include <d3dcompiler.h>
#include <list>

TMesh::TMesh(TVertex* vertices, size_t vertexCount, UINT* indices, size_t indexCount) {
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
    LOG_HR(device->CreateBuffer(&bd, &sd, &vertexBuffer))

    D3D11_BUFFER_DESC ibd = {};
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.CPUAccessFlags = 0u;
    ibd.MiscFlags = 0u;
    ibd.ByteWidth = indexCount * sizeof(UINT);
    ibd.StructureByteStride = sizeof(UINT);
    D3D11_SUBRESOURCE_DATA isd = {};
    isd.pSysMem = indices;
    LOG_HR(device->CreateBuffer(&ibd, &isd, &indexBuffer))

    float angle = 0.9f;

    cb =
    {
        {
            DirectX::XMMatrixTranspose(
                DirectX::XMMatrixRotationZ(angle) *
                DirectX::XMMatrixRotationX(angle) *
                DirectX::XMMatrixTranslation(0.0f,0.0f,4.0f) *
                DirectX::XMMatrixPerspectiveLH(1.0f,3.0f / 4.0f,0.5f,10.0f)
            )
        }
    };

    D3D11_BUFFER_DESC cbd;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbd.MiscFlags = 0u;
    cbd.ByteWidth = sizeof(cb);
    cbd.StructureByteStride = 0u;
    D3D11_SUBRESOURCE_DATA csd = {};
    csd.pSysMem = &cb;
    device->CreateBuffer(&cbd, &csd, &constantBuffer);

    context->VSSetConstantBuffers(0u, 1u, &constantBuffer);

    indicesToRender = indexCount;
}

TMesh::~TMesh() {
    RELEASE_COM(vertexBuffer)
    RELEASE_COM(indexBuffer)
}

void TMesh::Render() {
    const UINT stride = sizeof(TVertex);
    const UINT offset = 0u;

    context->IASetVertexBuffers(0, 1u, &vertexBuffer, &stride, &offset);
    context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0u);

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    context->DrawIndexed(indicesToRender, 0, 0);
}
