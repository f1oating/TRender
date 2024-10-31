#include "Mesh.h"
#include "stdexcept"

Mesh::Mesh() : indexCount(0) {}

Mesh::~Mesh() {}

void Mesh::SetVertices(const std::vector<Vertex>& vertices) {
    this->vertices = vertices;
}

void Mesh::SetIndices(const std::vector<unsigned int>& indices) {
    this->indices = indices;
    indexCount = static_cast<unsigned int>(indices.size());
}

void Mesh::InitializeBuffers(ID3D11Device* device) {
    // Vertex buffer creation
    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * static_cast<UINT>(vertices.size());
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = vertices.data();

    HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexData, vertexBuffer.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create vertex buffer");
    }

    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned int) * indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = indices.data();

    hr = device->CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create index buffer");
    }
}

void Mesh::Render(ID3D11DeviceContext* context) {
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    context->DrawIndexed(indexCount, 0, 0);
}
