#include "TRenderer.h"

/**
 * Constructor: Nothing spectacular.
 */
TRenderer::TRenderer(HINSTANCE hInst) 
{
    m_hInst = hInst;
    m_pDevice = nullptr;
    m_hDLL = NULL;
}
/*----------------------------------------------------------------*/

/**
 * Create the dll objects. This functions loads the appropriate dll.
 */
HRESULT TRenderer::CreateDevice(const char* chAPI) 
{
    char buffer[300];

    // decide which API should be used
    if (strcmp(chAPI, "Direct3D") == 0) 
    {
        m_hDLL = LoadLibrary(L"TD3D.dll");
        if (!m_hDLL) {
            // TODO: Error log
            return E_FAIL;
        }
    }
    else 
    {
        // TODO: Error log
        return E_FAIL;
    }

    GETRENDERDEVICE _GetRenderDevice = 0;
    HRESULT hr;

    // function pointer to dll's 'CreateRenderDevice' function
    _GetRenderDevice = (GETRENDERDEVICE)
        GetProcAddress(m_hDLL,
            "GetRenderDevice");

    // call dll's create function
    hr = _GetRenderDevice(&m_pDevice);
    if (FAILED(hr)) 
    {
        MessageBox(NULL,
            L"GetRenderDevice() from lib failed.",
            L"TEngine - error", MB_OK | MB_ICONERROR);
        m_pDevice = NULL;
        return E_FAIL;
    }

    return S_OK;
} // CreateDevice
/*----------------------------------------------------------------*/
