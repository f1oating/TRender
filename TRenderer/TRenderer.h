#ifndef TRENDERER_H
#define TRENDERER_H

#include <windows.h>
#include "TRenderDevice.h"

class TRenderer 
{
public:
    TRenderer(HINSTANCE hInst);
    ~TRenderer(void);

    HRESULT            CreateDevice(const char* chAPI);
    TRenderDevice*      GetDevice(void) { return m_pDevice; }
    HINSTANCE          GetModule(void) { return m_hDLL; }
    void               Release(void);

private:
    TRenderDevice* m_pDevice;
    HINSTANCE m_hInst;
    HMODULE m_hDLL;
};

#endif // TRENDERER_H
