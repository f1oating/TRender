#include "TRenderer.h"

#include <stdio.h>

TRenderer::TRenderer(HINSTANCE hInst) {
    m_hInst = hInst;
    m_pDevice = NULL;
    m_hDLL = NULL;
}

TRenderer::~TRenderer(void) {
    Release();
}

HRESULT TRenderer::CreateDevice(const char* chAPI) {
    char buffer[300];

    if (strcmp(chAPI, "Direct3D") == 0) {
        m_hDLL = LoadLibrary(L"TD3D.dll");
        if (!m_hDLL) {
            MessageBox(NULL,
                L"Loading TD3D.dll from lib failed.",
                L"TEngine - error", MB_OK | MB_ICONERROR);
            return E_FAIL;
        }
    }
    else {

        return E_FAIL;
    }

    CREATERENDERDEVICE _CreateRenderDevice = 0;
    HRESULT hr;

    _CreateRenderDevice = (CREATERENDERDEVICE)
        GetProcAddress(m_hDLL,
            "CreateRenderDevice");

    hr = _CreateRenderDevice(&m_pDevice);
    if (FAILED(hr)) {
        MessageBox(NULL,
            L"CreateRenderDevice() from lib failed.",
            L"TEngine - error", MB_OK | MB_ICONERROR);
        m_pDevice = NULL;
        return E_FAIL;
    }

    return S_OK;
}

void TRenderer::Release(void) {
    RELEASERENDERDEVICE _ReleaseRenderDevice = 0;
    HRESULT hr;

    if (m_hDLL) {
        _ReleaseRenderDevice = (RELEASERENDERDEVICE)
            GetProcAddress(m_hDLL, "ReleaseRenderDevice");
    }
    if (m_pDevice) {
        hr = _ReleaseRenderDevice(&m_pDevice);
        if (FAILED(hr)) {
            m_pDevice = NULL;
        }
    }
}
