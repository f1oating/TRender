#ifndef TD3D_H
#define TD3D_H

#include <d3d11.h>

D3D11_INPUT_ELEMENT_DESC MESH_INPUT_LAYOUT[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

D3D11_INPUT_ELEMENT_DESC SKYBOX_INPUT_LAYOUT[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

#endif
