#ifndef TRENDERDEVICE_H
#define TRENDERDEVICE_H

#include <windows.h>
#include <d3d11.h>
#include "T.h"

class TRenderDevice 
{
public:
    virtual ~TRenderDevice() {}

    virtual HRESULT Init(HWND hwnd, int width, int height, bool windowed) = 0;

    virtual void Release() = 0;

    virtual void BeginRendering() = 0;
    virtual void EndRendering() = 0;

    virtual void SetRenderState(RenderState state, bool value) = 0;

    virtual void ClearBuffers(float r, float g, float b, float a) = 0;
    virtual void SetViewport(int x, int y, int width, int height) = 0;

    virtual HRESULT UseWindow(HWND hwnd) = 0;

    virtual bool IsRunning() const = 0;

protected:
    HWND m_hWnd;
};

typedef TRenderDevice* LPTRENDERDEVICE;

extern "C" 
{
    HRESULT CreateRenderDevice(HINSTANCE hDLL, TRenderDevice** pInterface);
    typedef HRESULT(*CREATERENDERDEVICE)(HINSTANCE hDLL, TRenderDevice** pInterface);

    HRESULT ReleaseRenderDevice(TRenderDevice** pInterface);
    typedef HRESULT(*RELEASERENDERDEVICE)(TRenderDevice** pInterface);
}

#endif // TRENDERDEVICE_H
