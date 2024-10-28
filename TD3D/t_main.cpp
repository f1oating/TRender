#include "TD3D.h"

#include <stdexcept>
#include <d3dcompiler.h>

void TD3D::BeginRendering()
{
    m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);
    m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    float color[4] = { 0.5f, 0.2f, 0.4f, 1.0f };
    m_pContext->ClearRenderTargetView(m_pRenderTargetView, color);

    for (TMesh* mesh : m_pMeshes)
    {
        mesh->Render();
    }
}

void TD3D::EndRendering()
{
    m_pSwapChain->Present(1, 0);
}

void TD3D::LoadMesh(TVertex* vertices, size_t vertexCount, unsigned short* indices, size_t indexCount)
{
    TMesh* mesh = new TMesh(vertices, vertexCount, indices, indexCount);
    m_pMeshes.push_back(mesh);
}

bool TD3D::IsRunning() const
{
    return m_isRunning;
}
