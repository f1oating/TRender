#include "Mesh.h"
#include <fstream>

Mesh::Mesh() : indexCount(0) {}

Mesh::~Mesh() {}

bool Mesh::LoadFromFile(const std::string& filePath) {
    // Loading data from file.

    std::ifstream file(filePath);
    if (!file.is_open()) {
        return false;
    }

    int vertexCount, indexCount;
    file >> vertexCount >> indexCount;

    vertices.resize(vertexCount);
    for (int i = 0; i < vertexCount; ++i) {
        file >> vertices[i].position.x >> vertices[i].position.y >> vertices[i].position.z;
        file >> vertices[i].normal.x >> vertices[i].normal.y >> vertices[i].normal.z;
        file >> vertices[i].texcoord.x >> vertices[i].texcoord.y;
    }

    indices.resize(indexCount);
    for (int i = 0; i < indexCount; ++i) {
        file >> indices[i];
    }

    file.close();
    indexCount = static_cast<unsigned int>(indices.size());

    return true;
}

bool Mesh::InitializeBuffers(ID3D11Device* device) {
    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * static_cast<UINT>(vertices.size());
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = vertices.data();

    HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexData, vertexBuffer.GetAddressOf());
    if (FAILED(hr)) {
        return false;
    }

    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned int) * indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = indices.data();

    hr = device->CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.GetAddressOf());
    if (FAILED(hr)) {
        return false;
    }

    return true;
}

void Mesh::Render(ID3D11DeviceContext* context) {
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    context->DrawIndexed(indexCount, 0, 0);
}
