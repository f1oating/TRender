#ifndef SHADER_H
#define SHADER_H

#include <d3d11.h>
#include <wrl/client.h>
#include <string>

class Shader {
public:
    Shader();
    ~Shader();

    bool LoadFromFile(ID3D11Device* device, const std::string& filePath, const std::string& profile);
    void Bind(ID3D11DeviceContext* context);

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};

#endif // SHADER_H
