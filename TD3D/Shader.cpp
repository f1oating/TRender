#include "Shader.h"
#include <d3dcompiler.h>
#include <stdexcept>

Shader::Shader() : vertexShader(nullptr), pixelShader(nullptr) {}

Shader::~Shader() {
    vertexShader.Reset();
    pixelShader.Reset();
}

bool Shader::LoadFromFile(ID3D11Device* device, const std::string& filePath, const std::string& profile) {
    Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

    HRESULT hr = D3DReadFileToBlob(std::wstring(filePath.begin(), filePath.end()).c_str(), &shaderBlob);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to load shader from file: " + filePath);
    }

    if (profile == "vs_5_0") {
        hr = device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &vertexShader);
        if (FAILED(hr)) {
            throw std::runtime_error("Failed to create vertex shader from file: " + filePath);
        }

        D3D11_INPUT_ELEMENT_DESC layout[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        UINT numElements = ARRAYSIZE(layout);

        hr = device->CreateInputLayout(layout, numElements, shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout);
        if (FAILED(hr)) {
            throw std::runtime_error("Failed to create input layout from file: " + filePath);
        }
    }
    else if (profile == "ps_5_0") {
        hr = device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &pixelShader);
        if (FAILED(hr)) {
            throw std::runtime_error("Failed to create pixel shader from file: " + filePath);
        }
    }

    return true;
}

void Shader::Bind(ID3D11DeviceContext* context) {
    context->IASetInputLayout(inputLayout.Get());
    context->VSSetShader(vertexShader.Get(), nullptr, 0);
    context->PSSetShader(pixelShader.Get(), nullptr, 0);
}
