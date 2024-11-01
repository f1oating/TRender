#include "TDXRenderDevice.h"

#include "TDXMeshManager.h"
#include <stdexcept>
#include <d3dcompiler.h>

TDXRenderDevice::TDXRenderDevice() :
	m_pDevice(nullptr),
	m_pDeviceContext(nullptr),
	m_pSwapChain(nullptr),
	m_pRenderTargetView(nullptr),
	m_pDepthStencilView(nullptr),
	m_pDepthStencilBuffer(nullptr)
{
    m_TMeshManager = new TDXMeshManager(this);
}

bool TDXRenderDevice::Initizialize(HWND hwnd, int width, int height)
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, nullptr, 0,
        D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pDevice, &featureLevel, &m_pDeviceContext
    );

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create Direct3D device and swap chain.");
    }

    OnResize(width, height);

    D3DReadFileToBlob(L"..\\TD3D\\PixelShader.cso", &m_pBlob);
    hr = m_pDevice->CreatePixelShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_pPixelShader);

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create PixelShader.");
    }

    // bind pixel shader
    m_pDeviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);

    // create vertex shader
    D3DReadFileToBlob(L"..\\TD3D\\VertexShader.cso", &m_pBlob);
    hr = m_pDevice->CreateVertexShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_pVertexShader);

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create VertexShader.");
    }

    // bind vertex shader
    m_pDeviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);

    const D3D11_INPUT_ELEMENT_DESC ied[] =
    {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    hr = m_pDevice->CreateInputLayout(
        ied, (UINT)std::size(ied),
        m_pBlob->GetBufferPointer(),
        m_pBlob->GetBufferSize(),
        &m_pInputLayout
    );

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create InputLayout.");
    }

    m_pDeviceContext->IASetInputLayout(m_pInputLayout.Get());

    m_IsRunning = true;

    return true;
}

void TDXRenderDevice::BeginFrame(float r, float g, float b, float a) {
    float clearColor[] = { r, g, b, a };
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), clearColor);
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void TDXRenderDevice::EndFrame() {
    m_pSwapChain->Present(1, 0);
}

void TDXRenderDevice::Draw(TVertexColor* vertices, unsigned short numVertices)
{
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = numVertices * sizeof(TVertexColor);
    bd.StructureByteStride = sizeof(TVertexColor);

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;
    m_pDevice->CreateBuffer(&bd, &sd, &vertexBuffer);

    const UINT stride = sizeof(TVertexColor);
    const UINT offset = 0u;

    m_pDeviceContext->IASetVertexBuffers(0, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);

    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_pDeviceContext->Draw(numVertices, 0);
}

void TDXRenderDevice::Draw(TVertexColor* vertices, unsigned short numVertices, unsigned short* indices, unsigned short numIndices)
{
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = numVertices * sizeof(TVertexColor);
    bd.StructureByteStride = sizeof(TVertexColor);

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;
    m_pDevice->CreateBuffer(&bd, &sd, &vertexBuffer);

    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
    D3D11_BUFFER_DESC ibd = {};
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.CPUAccessFlags = 0u;
    ibd.MiscFlags = 0u;
    ibd.ByteWidth = numIndices * sizeof(unsigned short);
    ibd.StructureByteStride = sizeof(unsigned short);
    D3D11_SUBRESOURCE_DATA isd = {};
    isd.pSysMem = indices;
    m_pDevice->CreateBuffer(&ibd, &isd, &indexBuffer);

    const UINT stride = sizeof(TVertexColor);
    const UINT offset = 0u;

    m_pDeviceContext->IASetVertexBuffers(0, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);
    m_pDeviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_pDeviceContext->DrawIndexed(numIndices, 0, 0);
}

void TDXRenderDevice::SetProjectionMatrix(float width, float height, float farZ, float nearZ)
{

}

TMeshManager* TDXRenderDevice::GetMeshManager()
{
    return m_TMeshManager;
}

bool TDXRenderDevice::OnResize(int width, int height)
{
    m_pRenderTargetView.Reset();
    m_pDepthStencilBuffer.Reset();
    m_pDepthStencilView.Reset();

    HRESULT hr = m_pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to resize back buffer.");
    }

    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (&backBuffer));
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to get back buffer.");
    }

    hr = m_pDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_pRenderTargetView);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create render target view.");
    }

    D3D11_TEXTURE2D_DESC depthStencilDesc = {};
    depthStencilDesc.Width = width;
    depthStencilDesc.Height = height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    hr = m_pDevice->CreateTexture2D(&depthStencilDesc, nullptr, &m_pDepthStencilBuffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create depth stencil buffer.");
    }

    hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), nullptr, &m_pDepthStencilView);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create depth stencil view.");
    }

    m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    m_pDeviceContext->RSSetViewports(1, &viewport);

    return true;
}

bool TDXRenderDevice::IsRunning()
{
    return m_IsRunning;
}

HRESULT CreateRenderDevice(TDXRenderDevice** pDevice) {
    if (!*pDevice) {
        *pDevice = new TDXRenderDevice();
        return S_OK;
    }
    return 0x82000001;
}

HRESULT ReleaseRenderDevice(TDXRenderDevice** pDevice) {
    if (!*pDevice) {
        return 0x82000001;
    }
    delete* pDevice;
    *pDevice = nullptr;
    return S_OK;
}
