#ifndef TRENDERER_H
#define TRENDERER_H

#include <windows.h>
#include "TRenderDevice.h"

class TRenderer 
{
public:
    TRenderer(HINSTANCE hInst);

    HRESULT CreateDevice(const char* api);

    LPTRENDERDEVICE GetDevice() const { return m_pDevice; }

    HINSTANCE GetModule() const { return m_hDLL; }

private:
    LPTRENDERDEVICE m_pDevice;
    HINSTANCE m_hInst;
    HMODULE m_hDLL;
};

typedef TRenderer* LPTRENDERER;

#endif // TRENDERER_H
