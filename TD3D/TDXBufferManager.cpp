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

void TDXBufferManager::CreateStaticVertexBuffer(std::string name, void* vertices, unsigned int numVertices, unsigned int vertexSize, ID3D11Device* device)
{
    ID3D11Buffer* buffer;

    D3D11_BUFFER_DESC staticVertexBufferDesc = {};
    staticVertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    staticVertexBufferDesc.ByteWidth = vertexSize * numVertices;
    staticVertexBufferDesc.StructureByteStride = vertexSize;
    staticVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    staticVertexBufferDesc.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA staticVertexBufferInitData = {};
    staticVertexBufferInitData.pSysMem = vertices;
    HRESULT hr = device->CreateBuffer(&staticVertexBufferDesc, &staticVertexBufferInitData, &buffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create static vertex buffer.");
    }

    m_BuffersMap[name] = buffer;
}

void TDXBufferManager::CreateStaticIndexBuffer(std::string name, unsigned int* indices, unsigned int numIndices, ID3D11Device* device)
{
    ID3D11Buffer* buffer;

    D3D11_BUFFER_DESC staticIndexBufferDesc = {};
    staticIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    staticIndexBufferDesc.ByteWidth = sizeof(unsigned int) * numIndices;
    staticIndexBufferDesc.StructureByteStride = sizeof(unsigned int);
    staticIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    staticIndexBufferDesc.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA staticIndexBufferInitData = {};
    staticIndexBufferInitData.pSysMem = indices;
    HRESULT hr = device->CreateBuffer(&staticIndexBufferDesc, &staticIndexBufferInitData, &buffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create static index buffer.");
    }

    m_BuffersMap[name] = buffer;
}

void TDXBufferManager::CreateDynamicConstantBuffer(std::string name, unsigned int structSize, ID3D11Device* device)
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

void TDXBufferManager::CreateDynamicVertexBuffer(std::string name, void* vertices, unsigned int numVertices, unsigned int vertexSize, ID3D11Device* device)
{
    ID3D11Buffer* buffer;

    D3D11_BUFFER_DESC dynamicVertexBufferDesc = {};
    dynamicVertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    dynamicVertexBufferDesc.ByteWidth = vertexSize * numVertices;
    dynamicVertexBufferDesc.StructureByteStride = vertexSize;
    dynamicVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    dynamicVertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    D3D11_SUBRESOURCE_DATA dynamicVertexBufferInitData = {};
    dynamicVertexBufferInitData.pSysMem = vertices;
    HRESULT hr = device->CreateBuffer(&dynamicVertexBufferDesc, &dynamicVertexBufferInitData, &buffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create dynamic vertex buffer.");
    }

    m_BuffersMap[name] = buffer;
}

void TDXBufferManager::CreateDynamicIndexBuffer(std::string name, unsigned int* indices, unsigned int numIndices, ID3D11Device* device)
{
    ID3D11Buffer* buffer;

    D3D11_BUFFER_DESC dynamicIndexBufferDesc = {};
    dynamicIndexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    dynamicIndexBufferDesc.ByteWidth = sizeof(unsigned int) * numIndices;
    dynamicIndexBufferDesc.StructureByteStride = sizeof(unsigned int);
    dynamicIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    dynamicIndexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    D3D11_SUBRESOURCE_DATA dynamicIndexBufferInitData = {};
    dynamicIndexBufferInitData.pSysMem = indices;
    HRESULT hr = device->CreateBuffer(&dynamicIndexBufferDesc, &dynamicIndexBufferInitData, &buffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create dynamic index buffer.");
    }

    m_BuffersMap[name] = buffer;
}

void TDXBufferManager::CreateDynamicStructuredBuffer(std::string name, unsigned int structSize, unsigned int structCount, ID3D11Device* device)
{
    ID3D11Buffer* buffer;

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = structSize * structCount;
    bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    bd.StructureByteStride = structSize;

    HRESULT hr = device->CreateBuffer(&bd, nullptr, &buffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create Structured Buffer " + name);
    }

    m_BuffersMap[name] = buffer;
}

void TDXBufferManager::UpdateStaticVertexBuffer(std::string name, void* vertices, ID3D11DeviceContext* context)
{
    context->UpdateSubresource(m_BuffersMap[name], 0, nullptr, vertices, 0, 0);
}

void TDXBufferManager::UpdateStaticIndexBuffer(std::string name, unsigned int* indices, ID3D11DeviceContext* context)
{
    context->UpdateSubresource(m_BuffersMap[name], 0, nullptr, indices, 0, 0);
}

void TDXBufferManager::UpdateDynamicConstantBuffer(std::string name, void* constantBufferStruct, unsigned int structSize, ID3D11DeviceContext* context)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    context->Map(m_BuffersMap[name], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, constantBufferStruct, structSize);
    context->Unmap(m_BuffersMap[name], 0);
}

void TDXBufferManager::UpdateDynamicVertexBuffer(std::string name, void* vertices, unsigned int numVertices, unsigned int vertexSize, ID3D11DeviceContext* context)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource = {};
    HRESULT hr = context->Map(m_BuffersMap[name], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (SUCCEEDED(hr)) {
        memcpy(mappedResource.pData, vertices, vertexSize * numVertices);
        context->Unmap(m_BuffersMap[name], 0);
    }
}

void TDXBufferManager::UpdateDynamicIndexBuffer(std::string name, unsigned int* indices, unsigned int numIndices, ID3D11DeviceContext* context)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource = {};
    HRESULT hr = context->Map(m_BuffersMap[name], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (SUCCEEDED(hr)) {
        memcpy(mappedResource.pData, indices, sizeof(unsigned int) * numIndices);
        context->Unmap(m_BuffersMap[name], 0);
    }
}

void TDXBufferManager::UpdateDynamicStructuredBuffer(std::string name, void* structuredBufferStruct, unsigned int structSize, unsigned int structCount, ID3D11DeviceContext* context)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource = {};
    HRESULT hr = context->Map(m_BuffersMap[name], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (SUCCEEDED(hr)) {
        memcpy(mappedResource.pData, structuredBufferStruct, structSize * structCount);
        context->Unmap(m_BuffersMap[name], 0);
    }
}

void TDXBufferManager::BindVertexBuffer(std::string vertexName, UINT stride, UINT offset, ID3D11DeviceContext* context)
{
    context->IASetVertexBuffers(0, 1u, &m_BuffersMap[vertexName], &stride, &offset);
}

void TDXBufferManager::BindIndexBuffer(std::string indexName, ID3D11DeviceContext* context)
{
    context->IASetIndexBuffer(m_BuffersMap[indexName], DXGI_FORMAT_R32_UINT, 0u);
}

void TDXBufferManager::VBindConstantBuffer(std::string constantName, unsigned int slot, ID3D11DeviceContext* context)
{
    context->VSSetConstantBuffers(slot, 1, &m_BuffersMap[constantName]);
}

void TDXBufferManager::PBindConstantBuffer(std::string constantName, unsigned int slot, ID3D11DeviceContext* context)
{
    context->PSSetConstantBuffers(slot, 1, &m_BuffersMap[constantName]);
}

void TDXBufferManager::GBindConstantBuffer(std::string constantName, unsigned int slot, ID3D11DeviceContext* context)
{
    context->GSSetConstantBuffers(slot, 1, &m_BuffersMap[constantName]);
}

ID3D11Buffer* TDXBufferManager::GetBuffer(std::string name)
{
    return m_BuffersMap[name];
}

void TDXBufferManager::DeleteBuffer(std::string name)
{
    m_BuffersMap[name]->Release();
    m_BuffersMap.erase(name);
}
