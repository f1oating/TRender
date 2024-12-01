#include "TDXFeatureController.h"

void TDXFeatureController::Initizialize(ID3D11Device* device)
{
    CreateDepthStencilComparisons(device);
    CreateRasterizerCullings(device);
    CreateBlendStates(device);
}

void TDXFeatureController::ChangeDepthStencilComparison(bool flag, ID3D11DeviceContext* deviceContext)
{
    if (flag)
    {
        deviceContext->OMSetDepthStencilState(m_pDepthEnable.Get(), 1);
    }
    else
    {
        deviceContext->OMSetDepthStencilState(m_pDepthDisable.Get(), 1);
    }
}

void TDXFeatureController::ChangeRasterizerCulling(bool flag, ID3D11DeviceContext* deviceContext)
{
    if (flag)
    {
        deviceContext->RSSetState(m_pRasterizerStateEnable.Get());
    }
    else
    {
        deviceContext->RSSetState(m_pRasterizerStateDisable.Get());
    }
}

void TDXFeatureController::ChangeBlendState(bool flag, ID3D11DeviceContext* deviceContext)
{
    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    UINT sampleMask = 0xffffffff;

    if (flag)
    {
        deviceContext->OMSetBlendState(m_pBlendStateEnable.Get(), blendFactor, sampleMask);

    }
    else
    {
        deviceContext->OMSetBlendState(m_pBlendStateDisable.Get(), blendFactor, sampleMask);

    }
}

void TDXFeatureController::CreateDepthStencilComparisons(ID3D11Device* device)
{
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    device->CreateDepthStencilState(&depthStencilDesc, m_pDepthEnable.GetAddressOf());

    depthStencilDesc.DepthEnable = FALSE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

    device->CreateDepthStencilState(&depthStencilDesc, m_pDepthDisable.GetAddressOf());
}

void TDXFeatureController::CreateRasterizerCullings(ID3D11Device* device)
{
    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.FrontCounterClockwise = FALSE;

    device->CreateRasterizerState(&rasterizerDesc, m_pRasterizerStateEnable.GetAddressOf());

    rasterizerDesc.CullMode = D3D11_CULL_NONE;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.FrontCounterClockwise = TRUE;

    device->CreateRasterizerState(&rasterizerDesc, m_pRasterizerStateDisable.GetAddressOf());
}

void TDXFeatureController::CreateBlendStates(ID3D11Device* device)
{
    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.RenderTarget[0].BlendEnable = FALSE;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    device->CreateBlendState(&blendDesc, m_pBlendStateDisable.GetAddressOf());

    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    device->CreateBlendState(&blendDesc, m_pBlendStateEnable.GetAddressOf());
}
