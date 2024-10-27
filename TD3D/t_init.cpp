#include "TD3D.h"

#include <dxgi.h>
#include <DirectXMath.h>
#include <string>

TD3D TD3D::m_TD3D;

TD3D::TD3D()
    :
    m_pBoxVB(nullptr),
    m_pBoxIB(nullptr),
    m_pDevice(nullptr),
    m_pContext(nullptr),
    m_pSwapChain(nullptr),
    m_pDepthStencilBuffer(nullptr),
    m_pRenderTargetView(nullptr),
    m_pDepthStencilView(nullptr),
    m_pInputLayout(nullptr),
    m_isRunning(false)
{
    ZeroMemory(&m_pScreenViewport, sizeof(D3D11_VIEWPORT));
}

TD3D::~TD3D()
{
    Release();
}

HRESULT TD3D::Init(HWND hwnd, int width, int height, bool windowed)
{
    HRESULT hr;

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

    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = hwnd;
    sd.Windowed = true;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    IDXGIDevice* dxgiDevice = 0;
    m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

    IDXGIAdapter* dxgiAdapter = 0;
    dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

    IDXGIFactory* dxgiFactory = 0;
    dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

    dxgiFactory->CreateSwapChain(m_pDevice, &sd, &m_pSwapChain);

    if (dxgiDevice) { dxgiDevice->Release(); dxgiDevice = nullptr; }
    if (dxgiAdapter) { dxgiAdapter->Release(); dxgiAdapter = nullptr; }
    if (dxgiFactory) { dxgiFactory->Release(); dxgiFactory = nullptr; }

    OnResize(width, height);

    m_isRunning = true;
    return T_OK;
}

void TD3D::OnResize(int width, int height)
{
    if (m_pRenderTargetView) m_pRenderTargetView->Release(); m_pRenderTargetView = nullptr;
    if (m_pDepthStencilView) m_pDepthStencilView->Release(); m_pDepthStencilView = nullptr;
    if (m_pDepthStencilBuffer) m_pDepthStencilBuffer->Release(); m_pDepthStencilBuffer = nullptr;

    m_pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    ID3D11Texture2D* backBuffer;
    m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
    m_pDevice->CreateRenderTargetView(backBuffer, 0, &m_pRenderTargetView);
    if (backBuffer) backBuffer->Release(); backBuffer = nullptr;

    D3D11_TEXTURE2D_DESC depthStencilDesc;

    depthStencilDesc.Width = width;
    depthStencilDesc.Height = height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    m_pDevice->CreateTexture2D(&depthStencilDesc, 0, &m_pDepthStencilBuffer);
    m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, 0, &m_pDepthStencilView);

    m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);


    m_pScreenViewport.TopLeftX = 0;
    m_pScreenViewport.TopLeftY = 0;
    m_pScreenViewport.Width = static_cast<float>(width);
    m_pScreenViewport.Height = static_cast<float>(height);
    m_pScreenViewport.MinDepth = 0.0f;
    m_pScreenViewport.MaxDepth = 1.0f;

    m_pContext->RSSetViewports(1, &m_pScreenViewport);
}

void TD3D::Release()
{
    if (m_pRenderTargetView) { m_pRenderTargetView->Release(); m_pRenderTargetView = nullptr; }
    if (m_pDepthStencilView) { m_pDepthStencilView->Release(); m_pDepthStencilView = nullptr; }
    if (m_pDepthStencilBuffer) { m_pDepthStencilBuffer->Release(); m_pDepthStencilBuffer = nullptr; }
    if (m_pSwapChain) { m_pSwapChain->Release(); m_pSwapChain = nullptr; }
    if (m_pContext) { m_pContext->Release(); m_pContext = nullptr; }
    if (m_pDevice) { m_pDevice->Release(); m_pDevice = nullptr; }
    if (m_pInputLayout) { m_pInputLayout->Release(); m_pInputLayout = nullptr; }

    for (TMesh* mesh : m_pMeshes)
    {
        delete mesh;
    }
}

TD3D* TD3D::GetTD3D()
{
    return &m_TD3D;
}

HRESULT GetRenderDevice(TRenderDevice** pDevice) {
    if (!*pDevice) {
        *pDevice = TD3D::GetTD3D();
        return T_OK;
    }
    return T_FAIL;
}
/*----------------------------------------------------------------*/
