#ifndef TD3D_H
#define TD3D_H

#include "TRenderDevice.h"

#include <d3d11.h>
#include <d3dcompiler.h>

BOOL WINAPI DllEntryPoint(HINSTANCE hDll,
    DWORD     fdwReason,
    LPVOID    lpvRserved);



class TD3D : public TRenderDevice
{
public:
    TD3D(HINSTANCE);
    virtual ~TD3D();

    virtual HRESULT Init(HWND hwnd, int width, int height, bool windowed) override;
    virtual void Release() override;
    virtual void BeginRendering() override;
    virtual void EndRendering() override;
    virtual void SetRenderState(RenderState state, bool value) override;
    virtual void ClearBuffers(float r, float g, float b, float a) override;
    virtual void SetViewport(int x, int y, int width, int height) override;
    virtual HRESULT UseWindow(HWND hwnd) override;
    virtual bool IsRunning() const override;
    
private:
    HINSTANCE m_pHDLL;

    IDXGIFactory* m_pFactory;

    ID3D11Device* m_pDevice;
    ID3D11DeviceContext* m_pContext;
    IDXGISwapChain* m_pSwapChain;
    ID3D11RenderTargetView* m_pRenderTargetView;

    RenderState m_currentRenderState;
    bool m_isRunning;

    void CreateRenderTarget();

};

#endif
