#ifndef TD3D_H
#define TD3D_H

#include <d3d11.h>
#include <DirectXMath.h>

const DirectX::XMVECTOR DEFAULT_FORWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
const DirectX::XMVECTOR DEFAULT_UP_VECTOR = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
const DirectX::XMVECTOR DEFAULT_BACKWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
const DirectX::XMVECTOR DEFAULT_LEFT_VECTOR = DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
const DirectX::XMVECTOR DEFAULT_RIGHT_VECTOR = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

struct TDXCamera
{
    TDXCamera()
	{
        DirectX::XMVECTOR eye = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
        DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
        DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
        viewMatrix = DirectX::XMMatrixLookAtLH(eye, at, up);
        projectionMatrix = DirectX::XMMatrixIdentity();

        pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
        posVector = XMLoadFloat3(&pos);
        rot = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
        rotVector = XMLoadFloat3(&rot);
	}

	DirectX::XMMATRIX projectionMatrix;
	DirectX::XMMATRIX viewMatrix;

	DirectX::XMVECTOR posVector;
	DirectX::XMVECTOR rotVector;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 rot;
};

D3D11_INPUT_ELEMENT_DESC MESH_INPUT_LAYOUT[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

D3D11_INPUT_ELEMENT_DESC SKYBOX_INPUT_LAYOUT[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEX", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

#endif
