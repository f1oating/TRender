#include "TDXBufferManager.h"
#include <stdexcept>

TDXBufferManager::~TDXBufferManager()
{
    for (const std::pair<std::string, ID3D11Buffer*> pair : m_BuffersMap)
    {
        pair.second->Release();
    }

    m_BuffersMap.clear();
}

void TDXBufferManager::CreateStaticVertexBuffer(std::string name, void* vertices, unsigned short numVertices, unsigned short vertexSize, ID3D11Device* device)
{
    ID3D11Buffer* buffer;

    D3D11_BUFFER_DESC staticVertexBufferPTDesc = {};
    staticVertexBufferPTDesc.Usage = D3D11_USAGE_DEFAULT;
    staticVertexBufferPTDesc.ByteWidth = vertexSize * numVertices;
    staticVertexBufferPTDesc.StructureByteStride = vertexSize;
    staticVertexBufferPTDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    staticVertexBufferPTDesc.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA staticVertexBufferPTInitData = {};
    staticVertexBufferPTInitData.pSysMem = vertices;
    HRESULT hr = device->CreateBuffer(&staticVertexBufferPTDesc, &staticVertexBufferPTInitData, &buffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create static PT vertex buffer.");
    }

    m_BuffersMap[name] = buffer;
}

void TDXBufferManager::CreateStaticIndexBuffer(std::string name, unsigned short* indices, unsigned short numIndices, ID3D11Device* device)
{
    ID3D11Buffer* buffer;

    D3D11_BUFFER_DESC staticIndexBufferPTDesc = {};
    staticIndexBufferPTDesc.Usage = D3D11_USAGE_DEFAULT;
    staticIndexBufferPTDesc.ByteWidth = sizeof(unsigned short) * numIndices;
    staticIndexBufferPTDesc.StructureByteStride = sizeof(unsigned short);
    staticIndexBufferPTDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    staticIndexBufferPTDesc.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA staticIndexBufferPTInitData = {};
    staticIndexBufferPTInitData.pSysMem = indices;
    HRESULT hr = device->CreateBuffer(&staticIndexBufferPTDesc, &staticIndexBufferPTInitData, &buffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create static PT index buffer.");
    }

    m_BuffersMap[name] = buffer;
}

void TDXBufferManager::CreateDynamicConstantBuffer(std::string name, unsigned short structSize, ID3D11Device* device)
{
    ID3D11Buffer* buffer;

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = structSize;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    HRESULT hr = device->CreateBuffer(&bd, nullptr, &buffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create Constant Buffer " + name);
    }

    m_BuffersMap[name] = buffer;
}

void TDXBufferManager::UpdateStaticVertexBuffer(std::string name, void* vertices, ID3D11DeviceContext* context)
{
    context->UpdateSubresource(m_BuffersMap[name], 0, nullptr, vertices, 0, 0);
}

void TDXBufferManager::UpdateStaticIndexBuffer(std::string name, unsigned short* indices, ID3D11DeviceContext* context)
{
    context->UpdateSubresource(m_BuffersMap[name], 0, nullptr, indices, 0, 0);
}

void TDXBufferManager::UpdateDynamicConstantBuffer(std::string name, void* constantBufferStruct, unsigned short structSize, ID3D11DeviceContext* context)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    context->Map(m_BuffersMap[name], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, constantBufferStruct, structSize);
    context->Unmap(m_BuffersMap[name], 0);
}

void TDXBufferManager::BindVertexBuffer(std::string vertexName, UINT stride, UINT offset, ID3D11DeviceContext* context)
{
    context->IASetVertexBuffers(0, 1u, &m_BuffersMap[vertexName], &stride, &offset);
}

void TDXBufferManager::BindIndexBuffer(std::string indexName, ID3D11DeviceContext* context)
{
    context->IASetIndexBuffer(m_BuffersMap[indexName], DXGI_FORMAT_R16_UINT, 0u);
}

void TDXBufferManager::BindConstantBuffer(std::string constantName, unsigned short slot, ID3D11DeviceContext* context)
{
    context->VSSetConstantBuffers(slot, 1, &m_BuffersMap[constantName]);
}

void TDXBufferManager::DeleteBuffer(std::string name)
{
    m_BuffersMap[name]->Release();
    m_BuffersMap.erase(name);
}
