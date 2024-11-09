#include "TDXFeatureController.h"

void TDXFeatureController::ChangeDepthStencilComparison(D3D11_COMPARISON_FUNC comparisonFunc, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
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
    depthStencilDesc.DepthFunc = comparisonFunc;
    depthStencilDesc.StencilEnable = FALSE;

    ID3D11DepthStencilState* newDepthStencilState;
    device->CreateDepthStencilState(&depthStencilDesc, &newDepthStencilState);

    deviceContext->OMSetDepthStencilState(newDepthStencilState, 1);

    if (newDepthStencilState)
    {
        newDepthStencilState->Release();
    }
}

void TDXFeatureController::ChangeRasterizerCulling(D3D11_CULL_MODE cullMode, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
    ID3D11RasterizerState* currentRasterizerState = nullptr;
    deviceContext->RSGetState(&currentRasterizerState);

    if (currentRasterizerState)
    {
        currentRasterizerState->Release();
    }

    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.CullMode = cullMode;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.FrontCounterClockwise = TRUE;

    ID3D11RasterizerState* newRasterizerState;
    device->CreateRasterizerState(&rasterizerDesc, &newRasterizerState);

    deviceContext->RSSetState(newRasterizerState);

    if (newRasterizerState)
    {
        newRasterizerState->Release();
    }
}
