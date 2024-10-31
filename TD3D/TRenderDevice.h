#ifndef TRENDERDEVICE_H
#define TRENDERDEVICE_H

#include <d3d11.h>
#include <wrl/client.h>

class TRenderDevice {
protected:
    TRenderDevice();
    static TRenderDevice m_TRenderDevice;

public:
    TRenderDevice();
    ~TRenderDevice();

    bool Initialize(HWND hwnd, int width, int height);
    void BeginFrame(float red, float green, float blue, float alpha);
    void EndFrame();

    ID3D11Device* GetDevice() const;
    ID3D11DeviceContext* GetDeviceContext() const;

    bool OnResize(int width, int height);

    static TRenderDevice* GetRenderDevice();

private:
    bool InitializeDirect3D(HWND hwnd, int width, int height);

    Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;

};

#endif // TRENDERDEVICE_H
