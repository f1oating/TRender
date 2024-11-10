#include "TDXFeatureController.h"

void TDXFeatureController::ChangeDepthStencilComparison(bool flag, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
    ID3D11DepthStencilState* currentDepthStencilState = nullptr;
    deviceContext->OMGetDepthStencilState(&currentDepthStencilState, nullptr);

    if (currentDepthStencilState)
    {
        currentDepthStencilState->Release();
    }

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    if (!flag)
    {
        depthStencilDesc.DepthEnable = FALSE;
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
    }

    ID3D11DepthStencilState* newDepthStencilState;
    device->CreateDepthStencilState(&depthStencilDesc, &newDepthStencilState);

    deviceContext->OMSetDepthStencilState(newDepthStencilState, 1);

    if (newDepthStencilState)
    {
        newDepthStencilState->Release();
    }
}

void TDXFeatureController::ChangeRasterizerCulling(bool flag, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
    ID3D11RasterizerState* currentRasterizerState = nullptr;
    deviceContext->RSGetState(&currentRasterizerState);

    if (currentRasterizerState)
    {
        currentRasterizerState->Release();
    }

    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.FrontCounterClockwise = FALSE;

    if (!flag)
    {
        rasterizerDesc.CullMode = D3D11_CULL_NONE;
        rasterizerDesc.FillMode = D3D11_FILL_SOLID;
        rasterizerDesc.FrontCounterClockwise = TRUE;
    }

    ID3D11RasterizerState* newRasterizerState;
    device->CreateRasterizerState(&rasterizerDesc, &newRasterizerState);

    deviceContext->RSSetState(newRasterizerState);

    if (newRasterizerState)
    {
        newRasterizerState->Release();
    }
}
