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