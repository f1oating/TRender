#include "TDXShaderManager.h"
#include <stdexcept>
#include <d3dcompiler.h>

TDXShaderManager::TDXShaderManager()
{
}

TDXShaderManager::~TDXShaderManager()
{
    for (const std::pair<std::string, ID3D11VertexShader*> pair : m_VertexShadersMap)
    {
        pair.second->Release();
    }
    for (const std::pair<std::string, ID3D11PixelShader*> pair : m_PixelShadersMap)
    {
        pair.second->Release();
    }
    for (const std::pair<std::string, ID3D11GeometryShader*> pair : m_GeometryShadersMap)
    {
        pair.second->Release();
    }
    for (const std::pair<std::string, ID3D11InputLayout*> pair : m_InputLayoutsMap)
    {
        pair.second->Release();
    }

    m_VertexShadersMap.clear();
    m_PixelShadersMap.clear();
    m_InputLayoutsMap.clear();
}

void TDXShaderManager::AddVertexShader(std::string name, LPCWSTR path, D3D11_INPUT_ELEMENT_DESC* ied, UINT size, ID3D11Device* device)
{
    ID3D11VertexShader* vertexShader;
    ID3D11InputLayout* inputLayout;

    D3DReadFileToBlob(path, &m_pBlob);
    HRESULT hr = device->CreateVertexShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &vertexShader);

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create VertexShader.");
    }

    hr = device->CreateInputLayout(
        ied, size,
        m_pBlob->GetBufferPointer(),
        m_pBlob->GetBufferSize(),
        &inputLayout
    );

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create InputLayout.");
    }

    m_VertexShadersMap[name] = vertexShader;
    m_InputLayoutsMap[name] = inputLayout;
}

void TDXShaderManager::AddPixelShader(std::string name, LPCWSTR path, ID3D11Device* device)
{
    ID3D11PixelShader* pixelShader;

    D3DReadFileToBlob(path, &m_pBlob);
    HRESULT hr = device->CreatePixelShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &pixelShader);

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create PixelShader.");
    }

    m_PixelShadersMap[name] = pixelShader;
}

void TDXShaderManager::AddGeometryShader(std::string name, LPCWSTR path, ID3D11Device* device)
{
    ID3D11GeometryShader* geometryShader;

    D3DReadFileToBlob(path, &m_pBlob);
    HRESULT hr = device->CreateGeometryShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &geometryShader);

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create GeometryShader.");
    }

    m_GeometryShadersMap[name] = geometryShader;
}

void TDXShaderManager::BindVertexShader(std::string name, ID3D11DeviceContext* deviceContext)
{
    deviceContext->VSSetShader(m_VertexShadersMap[name], nullptr, 0u);
    deviceContext->IASetInputLayout(m_InputLayoutsMap[name]);
}

void TDXShaderManager::BindPixelShader(std::string name, ID3D11DeviceContext* deviceContext)
{
    deviceContext->PSSetShader(m_PixelShadersMap[name], nullptr, 0u);
}

void TDXShaderManager::BindGeometryShader(std::string name, ID3D11DeviceContext* deviceContext)
{
    deviceContext->GSSetShader(m_GeometryShadersMap[name], nullptr, 0u);
}

void TDXShaderManager::UnbindVertexShader(std::string name, ID3D11DeviceContext* deviceContext)
{
    deviceContext->VSSetShader(nullptr, nullptr, 0u);
    deviceContext->IASetInputLayout(nullptr);
}

void TDXShaderManager::UnbindPixelShader(std::string name, ID3D11DeviceContext* deviceContext)
{
    deviceContext->PSSetShader(nullptr, nullptr, 0u);
}

void TDXShaderManager::UnbindGeometryShader(std::string name, ID3D11DeviceContext* deviceContext)
{
    deviceContext->GSSetShader(nullptr, nullptr, 0u);
}

ID3D11VertexShader* TDXShaderManager::GetVertexShader(std::string name)
{
    return m_VertexShadersMap[name];
}

ID3D11PixelShader* TDXShaderManager::GetPixelShader(std::string name)
{
    return m_PixelShadersMap[name];
}

ID3D11GeometryShader* TDXShaderManager::GetGeometryShader(std::string name)
{
    return m_GeometryShadersMap[name];
}
