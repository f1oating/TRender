#include "TMesh.h"

#include <DirectXMath.h>

TMesh::TMesh(ID3D11Device* device, TVertex* verticies, UINT numVerticies, UINT* indicies, UINT numIndicies) {
    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(TVertex) * numVerticies;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexData;
    ZeroMemory(&vertexData, sizeof(vertexData));
    vertexData.pSysMem = verticies;

    HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(UINT) * numIndicies;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA indexData;
    ZeroMemory(&indexData, sizeof(indexData));
    indexData.pSysMem = indicies;

    hr = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

    indexCount = numIndicies;
}

TMesh::~TMesh() {
    if (vertexBuffer) vertexBuffer->Release();
    if (indexBuffer) indexBuffer->Release();
}

void TMesh::Render(ID3D11DeviceContext* context) {
    UINT stride = sizeof(TVertex);
    UINT offset = 0;

    context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->DrawIndexed(indexCount, 0, 0);
}
