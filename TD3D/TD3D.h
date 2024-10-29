#ifndef TD3D_H
#define TD3D_H

#include "TRenderDevice.h"
#include "TMesh.h"
#include "TD3D_MACROS.h"
#include "TLogManager.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <list>
#include <DirectXMath.h>

BOOL WINAPI DllEntryPoint(HINSTANCE hDll,
    DWORD     fdwReason,
    LPVOID    lpvRserved);

class TD3D : public TRenderDevice
{
protected:
    TD3D();
    static TD3D m_TD3D;

public:
    virtual ~TD3D();

    virtual HRESULT Init(HWND hwnd, int width, int height, bool windowed) override;
    virtual void BeginRendering() override;
    virtual void EndRendering() override;
    virtual void OnResize(int width, int height) override;
    virtual void LoadMesh(TVertex* vertices, size_t vertexCount, UINT* indices, size_t indexCount) override;
    virtual bool IsRunning() const override;

    ID3D11Device* GetDevice() { return m_pDevice;  };
    ID3D11DeviceContext* GetDeviceContext() { return m_pContext; };

    static TD3D* GetTD3D();
    
private:
    ID3D11Device* m_pDevice;
    ID3D11DeviceContext* m_pContext;
    IDXGISwapChain* m_pSwapChain;
    ID3D11Texture2D* m_pDepthStencilBuffer;
    ID3D11RenderTargetView* m_pRenderTargetView;
    ID3D11DepthStencilView* m_pDepthStencilView;

    ID3D11VertexShader* m_pVertexShader;
    ID3D11PixelShader* m_pPixelShader;
    ID3DBlob* m_pBlob;
    ID3D11InputLayout* m_pInputLayout;

    D3D11_VIEWPORT m_pScreenViewport;

    bool m_isRunning;

    std::list<TMesh*> m_pMeshes;

    void Release();
    void BuildShaders();

};

void LogHRMessage(HRESULT hr);
void LogShaderErrorMessage(const char* header, ID3DBlob* blob);

#endif
