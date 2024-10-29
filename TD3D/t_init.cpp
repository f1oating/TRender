#include "TD3D.h"

#include <dxgi.h>
#include <DirectXMath.h>
#include <string>

TD3D TD3D::m_TD3D;

TD3D::TD3D()
    :
    m_pDevice(nullptr),
    m_pContext(nullptr),
    m_pSwapChain(nullptr),
    m_pDepthStencilBuffer(nullptr),
    m_pRenderTargetView(nullptr),
    m_pDepthStencilView(nullptr),
    m_pVertexShader(nullptr),
    m_pPixelShader(nullptr),
    m_pBlob(nullptr),
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
    RETURN_HR(D3D11CreateDevice(
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
    ))

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
    RETURN_HR(m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice))

    IDXGIAdapter* dxgiAdapter = 0;
    RETURN_HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter))

    IDXGIFactory* dxgiFactory = 0;
    RETURN_HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory))

    RETURN_HR(dxgiFactory->CreateSwapChain(m_pDevice, &sd, &m_pSwapChain));

    RELEASE_COM(dxgiDevice)
    RELEASE_COM(dxgiAdapter)
    RELEASE_COM(dxgiFactory)

    OnResize(width, height);
    BuildShaders();

    m_isRunning = true;
    return T_OK;
}

void TD3D::BuildShaders()
{
    LOG_HR(D3DReadFileToBlob(L"..\\TD3D\\PixelShader.cso", &m_pBlob), m_pBlob)
    LOG_HR(m_pDevice->CreatePixelShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_pPixelShader))

    // bind pixel shader
    m_pContext->PSSetShader(m_pPixelShader, nullptr, 0u);

    // create vertex shader
    LOG_HR(D3DReadFileToBlob(L"..\\TD3D\\VertexShader.cso", &m_pBlob), m_pBlob)
    LOG_HR(m_pDevice->CreateVertexShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_pVertexShader))

    // bind vertex shader
    m_pContext->VSSetShader(m_pVertexShader, nullptr, 0u);

    // input (vertex) layout (2d position only)
    const D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        { "Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
        { "Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,8u,D3D11_INPUT_PER_VERTEX_DATA,0 },
    };
    LOG_HR(m_pDevice->CreateInputLayout(
        ied, (UINT)std::size(ied),
        m_pBlob->GetBufferPointer(),
        m_pBlob->GetBufferSize(),
        &m_pInputLayout
    ))

    m_pContext->IASetInputLayout(m_pInputLayout);
}

void TD3D::OnResize(int width, int height)
{
    RELEASE_COM(m_pRenderTargetView)
    RELEASE_COM(m_pDepthStencilView)
    RELEASE_COM(m_pDepthStencilBuffer)

    LOG_HR(m_pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0))
    ID3D11Texture2D* backBuffer;
    LOG_HR(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)))
    LOG_HR(m_pDevice->CreateRenderTargetView(backBuffer, 0, &m_pRenderTargetView))
    RELEASE_COM(backBuffer)

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

    LOG_HR(m_pDevice->CreateTexture2D(&depthStencilDesc, 0, &m_pDepthStencilBuffer))
    LOG_HR(m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, 0, &m_pDepthStencilView))

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
    RELEASE_COM(m_pDevice)
    RELEASE_COM(m_pContext)
    RELEASE_COM(m_pSwapChain)
    RELEASE_COM(m_pDepthStencilBuffer)
    RELEASE_COM(m_pRenderTargetView)
    RELEASE_COM(m_pDepthStencilView)
    RELEASE_COM(m_pVertexShader)
    RELEASE_COM(m_pPixelShader)
    RELEASE_COM(m_pBlob)
    RELEASE_COM(m_pInputLayout)

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
