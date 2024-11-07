#include "TDXTextureManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdexcept>

TDXTextureManager::TDXTextureManager()
{

}

TDXTextureManager::~TDXTextureManager()
{

}

void TDXTextureManager::CreateSampler(ID3D11Device* device, ID3D11DeviceContext* context)
{
    ID3D11SamplerState* pSamplerState;

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0; 
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    HRESULT hr = device->CreateSamplerState(&samplerDesc, &pSamplerState);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create sampler!");
        return;
    }

    context->PSSetSamplers(0, 1, &pSamplerState);
}

void TDXTextureManager::AddTexture(std::string name, std::string path, ID3D11Device* device)
{
    int width, height, channels;
    unsigned char* imageData = stbi_load(path.c_str(), &width, &height, &channels, 4);
    if (!imageData) {
        throw std::runtime_error("Failed to load texture!");
    }

    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = imageData;
    initData.SysMemPitch = width * 4;

    ID3D11Texture2D* pTexture = nullptr;
    HRESULT hr = device->CreateTexture2D(&textureDesc, &initData, &pTexture);
    if (FAILED(hr)) {
        stbi_image_free(imageData);
        throw std::runtime_error("Failed to create texture!");
    }

    ID3D11ShaderResourceView* pTextureView = nullptr;
    hr = device->CreateShaderResourceView(pTexture, nullptr, &pTextureView);
    if (FAILED(hr)) {
        pTexture->Release();
        stbi_image_free(imageData);
        throw std::runtime_error("Failed to create shader resource view!");
    }

    m_TexturesMap[name] = pTextureView;
}

void TDXTextureManager::BindTexture(std::string name, ID3D11DeviceContext* context)
{
    context->PSSetShaderResources(0, 1, &m_TexturesMap[name]);
}

void TDXTextureManager::DeleteTexture(std::string name)
{
    if (m_TexturesMap[name])
    {
        delete m_TexturesMap[name];
        m_TexturesMap.erase(name);
    }
}
