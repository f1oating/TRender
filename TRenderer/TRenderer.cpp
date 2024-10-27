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
 * Destructor: Just call the Release method
 */
TRenderer::~TRenderer(void) 
{
    Release();
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

    CREATERENDERDEVICE _CreateRenderDevice = 0;
    HRESULT hr;

    // function pointer to dll's 'CreateRenderDevice' function
    _CreateRenderDevice = (CREATERENDERDEVICE)
        GetProcAddress(m_hDLL,
            "CreateRenderDevice");

    // call dll's create function
    hr = _CreateRenderDevice(m_hDLL, &m_pDevice);
    if (FAILED(hr)) 
    {
        MessageBox(NULL,
            L"CreateRenderDevice() from lib failed.",
            L"TEngine - error", MB_OK | MB_ICONERROR);
        m_pDevice = NULL;
        return E_FAIL;
    }

    return S_OK;
} // CreateDevice
/*----------------------------------------------------------------*/

/**
 * Cleanup the dll objects.
 */
void TRenderer::Release(void) 
{
    RELEASERENDERDEVICE _ReleaseRenderDevice = 0;
    HRESULT hr;

    if (m_hDLL) 
    {
        // function pointer to dll 'ReleaseRenderDevice' function
        _ReleaseRenderDevice = (RELEASERENDERDEVICE)
            GetProcAddress(m_hDLL, "ReleaseRenderDevice");
    }
    // call dll's release function
    if (m_pDevice) 
    {
        hr = _ReleaseRenderDevice(&m_pDevice);
        if (FAILED(hr)) 
        {
            m_pDevice = NULL;
        }
    }
} // Release
/*----------------------------------------------------------------*/
