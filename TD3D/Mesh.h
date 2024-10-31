#ifndef MESH_H
#define MESH_H

#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <vector>
#include <string>

struct Vertex {
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT2 texcoord;
};

class Mesh {
public:
    Mesh();
    ~Mesh();

    bool LoadFromFile(const std::string& filePath);
    void Render(ID3D11DeviceContext* context);

private:
    bool InitializeBuffers(ID3D11Device* device);

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
    unsigned int indexCount;
};

#endif // MESH_H
