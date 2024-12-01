#include "TDXTextureManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdexcept>
#include <vector>
#include <string>

std::vector<std::string> CUBE_MAP_SIDES = {
    "right", "left", "top", "bottom", "front", "back"
};

TDXTextureManager::TDXTextureManager()
{

}

TDXTextureManager::~TDXTextureManager()
{
    for (const std::pair<std::string, ID3D11ShaderResourceView*> pair : m_TexturesMap)
    {
        pair.second->Release();
    }

    for (const std::pair<std::string, ID3D11SamplerState*> pair : m_SamplersMap)
    {
        pair.second->Release();
    }

    m_TexturesMap.clear();
    m_SamplersMap.clear();
}

void TDXTextureManager::CreateSampler(std::string name, ID3D11Device* device, ID3D11DeviceContext* context)
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

    m_SamplersMap[name] = pSamplerState;
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
        throw std::runtime_error("Failed to create shader resource view!");
    }

    m_TexturesMap[name] = pTextureView;
}

void TDXTextureManager::AddTexture(std::string name, unsigned int r, unsigned int g, unsigned int b, unsigned int a, ID3D11Device* device)
{
    unsigned int colorPixel = (a << 24) |
        (b << 16) |
        (g << 8) |
        r;

    D3D11_SUBRESOURCE_DATA initData = { &colorPixel, sizeof(unsigned int), 0 };

    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = texDesc.Height = 1;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.Usage = D3D11_USAGE_IMMUTABLE;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;

    ID3D11Texture2D* colorTexture = nullptr;
    HRESULT hr = device->CreateTexture2D(&texDesc, &initData, &colorTexture);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create texture for color!");
    }

    ID3D11ShaderResourceView* colorSRV = nullptr;
    hr = device->CreateShaderResourceView(colorTexture, nullptr, &colorSRV);
    if (FAILED(hr))
    {
        colorTexture->Release();
        throw std::runtime_error("Failed to create Shader Resource View for color!");
    }

    m_TexturesMap[name] = colorSRV;

    colorTexture->Release();
}

void TDXTextureManager::AddCubeMapTexture(std::string name, std::string path, std::string ext, ID3D11Device* device)
{
    int width, height, channels;
    std::vector<unsigned char*> facesData;

    for (std::string& filename : CUBE_MAP_SIDES) {
        unsigned char* data = LoadCubeMapTextureData(path + "_" + filename + ext, width, height, channels);
        if (!data) {
            for (unsigned char* face : facesData) stbi_image_free(face);
        }
        facesData.push_back(data);
    }

    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 6;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

    D3D11_SUBRESOURCE_DATA subResourceData[6] = {};
    for (int i = 0; i < 6; ++i) {
        subResourceData[i].pSysMem = facesData[i];
        subResourceData[i].SysMemPitch = width * 4;
    }

    ID3D11Texture2D* cubeTexture = nullptr;
    HRESULT hr = device->CreateTexture2D(&textureDesc, subResourceData, &cubeTexture);

    for (unsigned char* face : facesData) stbi_image_free(face);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    srvDesc.TextureCube.MipLevels = 1;
    srvDesc.TextureCube.MostDetailedMip = 0;

    ID3D11ShaderResourceView* pTextureView;
    hr = device->CreateShaderResourceView(cubeTexture, &srvDesc, &pTextureView);

    m_TexturesMap[name] = pTextureView;
}

void TDXTextureManager::BindTexture(std::string name, ID3D11DeviceContext* context)
{
    context->PSSetShaderResources(0, 1, &m_TexturesMap[name]);
}

void TDXTextureManager::BindSampler(std::string name, ID3D11DeviceContext* context)
{
    context->PSSetSamplers(0, 1, &m_SamplersMap[name]);

}

void TDXTextureManager::DeleteTexture(std::string name)
{
    if (m_TexturesMap[name])
    {
        delete m_TexturesMap[name];
        m_TexturesMap.erase(name);
    }
}

unsigned char* TDXTextureManager::LoadCubeMapTextureData(const std::string& filename, int& width, int& height, int& channels)
{
    stbi_set_flip_vertically_on_load(false);
    return stbi_load(filename.c_str(), &width, &height, &channels, 4);
}
