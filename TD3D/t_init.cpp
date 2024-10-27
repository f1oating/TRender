#include "TD3D.h"

#include <dxgi.h>
#include <DirectXMath.h>
#include <stdexcept>

TD3D::TD3D(HINSTANCE hDLL)
    : m_pHDLL(hDLL),
    m_pFactory(nullptr),
    m_pDevice(nullptr),
    m_pContext(nullptr),
    m_pSwapChain(nullptr),
    m_pRenderTargetView(nullptr),
    m_currentRenderState(RenderState::Solid),
    m_isRunning(false)
{
}

TD3D::~TD3D()
{
    Release();
}

HRESULT TD3D::Init(HWND hwnd, int width, int height, bool windowed)
{
    HRESULT hr;

    hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_pFactory);
    if (FAILED(hr))
    {
        return hr;
    }

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevel;
    hr = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createDeviceFlags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &m_pDevice,
        &featureLevel,
        &m_pContext
    );

    if (FAILED(hr))
    {
        return hr;
    }

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = windowed;

    hr = m_pFactory->CreateSwapChain(m_pDevice, &swapChainDesc, &m_pSwapChain);
    if (FAILED(hr))
    {
        return hr;
    }

    CreateRenderTarget();

    m_isRunning = true;
    return S_OK;
}

void TD3D::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to get back buffer.");
    }

    hr = m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
    pBackBuffer->Release();

    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create render target view.");
    }

    m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);
}

void TD3D::Release()
{
    if (m_pRenderTargetView) { m_pRenderTargetView->Release(); m_pRenderTargetView = nullptr; }
    if (m_pSwapChain) { m_pSwapChain->Release(); m_pSwapChain = nullptr; }
    if (m_pContext) { m_pContext->Release(); m_pContext = nullptr; }
    if (m_pDevice) { m_pDevice->Release(); m_pDevice = nullptr; }
}

void TD3D::BeginRendering()
{
    float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
    m_pContext->ClearRenderTargetView(m_pRenderTargetView, color);
}

void TD3D::EndRendering()
{
    m_pSwapChain->Present(1, 0);
}

void TD3D::SetRenderState(RenderState state, bool value)
{
    m_currentRenderState = state;
}

void TD3D::ClearBuffers(float r, float g, float b, float a)
{
    float clearColor[4] = { r, g, b, a };
    m_pContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
}

void TD3D::SetViewport(int x, int y, int width, int height)
{
    D3D11_VIEWPORT viewport;
    viewport.Width = static_cast<FLOAT>(width);
    viewport.Height = static_cast<FLOAT>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = static_cast<FLOAT>(x);
    viewport.TopLeftY = static_cast<FLOAT>(y);

    m_pContext->RSSetViewports(1, &viewport);
}

HRESULT TD3D::UseWindow(HWND hwnd)
{
    // TODO: implement
    return S_OK;
}

bool TD3D::IsRunning() const
{
    return m_isRunning;
}

HRESULT CreateRenderDevice(HINSTANCE hDLL, TRenderDevice** pDevice) {
    if (!*pDevice) {
        *pDevice = new TD3D(hDLL);
        return S_OK;
    }
    return 0x82000001;
}
/*----------------------------------------------------------------*/

/**
 * Exported release function: Realeses the given ZFXRenderDevice object.
 */
HRESULT ReleaseRenderDevice(TRenderDevice** pDevice) {
    if (!*pDevice) {
        return 0x82000001;
    }
    delete* pDevice;
    *pDevice = nullptr;
    return S_OK;
}
