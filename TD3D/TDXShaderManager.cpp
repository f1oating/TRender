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
    for (const std::pair<std::string, ID3D11InputLayout*> pair : m_InputLayoutsMap)
    {
        pair.second->Release();
    }

    m_VertexShadersMap.clear();
    m_PixelShadersMap.clear();
    m_InputLayoutsMap.clear();
}

void TDXShaderManager::AddShaders(std::string name, LPCWSTR pathVertex, LPCWSTR pathPixel, D3D11_INPUT_ELEMENT_DESC* ied, UINT size, ID3D11Device* device)
{
    ID3D11VertexShader* vertexShader;
    ID3D11PixelShader* pixelShader;
    ID3D11InputLayout* inputLayout;


    D3DReadFileToBlob(pathPixel, &m_pBlob);
    HRESULT hr = device->CreatePixelShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &pixelShader);

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create PixelShader.");
    }

    D3DReadFileToBlob(pathVertex, &m_pBlob);
    hr = device->CreateVertexShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &vertexShader);

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
    m_PixelShadersMap[name] = pixelShader;
    m_InputLayoutsMap[name] = inputLayout;
}

void TDXShaderManager::BindShaders(std::string name, ID3D11DeviceContext* deviceContext)
{
    deviceContext->VSSetShader(m_VertexShadersMap[name], nullptr, 0u);
    deviceContext->PSSetShader(m_PixelShadersMap[name], nullptr, 0u);
    deviceContext->IASetInputLayout(m_InputLayoutsMap[name]);
}

ID3D11VertexShader* TDXShaderManager::GetVertexShader(std::string name)
{
    return m_VertexShadersMap[name];
}

ID3D11PixelShader* TDXShaderManager::GetPixelShader(std::string name)
{
    return m_PixelShadersMap[name];
}
