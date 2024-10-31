#include "Texture.h"
#include <WICTextureLoader.h>
#include <stdexcept>

bool Texture::LoadFromFile(const std::string& path, ID3D11Device* device) {
    std::wstring wPath(path.begin(), path.end());

    HRESULT hr = DirectX::CreateWICTextureFromFile(device, wPath.c_str(), nullptr, textureView.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to load texture from file: " + path);
    }
    return true;
}

void Texture::Bind(ID3D11DeviceContext* context, int slot) {
    context->PSSetShaderResources(slot, 1, textureView.GetAddressOf());
}
