#ifndef TEXTURE_H
#define TEXTURE_H

#include <d3d11.h>
#include <wrl/client.h>
#include <string>

class Texture {
public:
    bool LoadFromFile(const std::string& path, ID3D11Device* device);
    void Bind(ID3D11DeviceContext* context, int slot = 0);

private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;
};

#endif // TEXTURE_H
