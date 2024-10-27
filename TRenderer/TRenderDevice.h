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

    virtual void BeginRendering() = 0;
    virtual void EndRendering() = 0;

    virtual void OnResize(int width, int height) = 0;

    virtual void LoadMesh(TVertex verticies[], UINT indicies[]) = 0;

    virtual bool IsRunning() const = 0;

protected:
    HWND m_hWnd;

};

typedef TRenderDevice* LPTRENDERDEVICE;

extern "C" 
{
    HRESULT GetRenderDevice(TRenderDevice** pInterface);
    typedef HRESULT(*GETRENDERDEVICE)(TRenderDevice** pInterface);
}

#endif // TRENDERDEVICE_H
