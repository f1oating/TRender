#include "TRenderer.h"

TRenderer::TRenderer(HINSTANCE hInst) 
{
    m_hInst = hInst;

    m_hDLL = LoadLibrary(L"TD3D.dll");
}

TRenderDevice* TRenderer::GetRenderDevice()
{
    GetTRenderDeviceFunc _GetTRenderDevice = 0;

    _GetTRenderDevice = (GetTRenderDeviceFunc) GetProcAddress(m_hDLL, "GetTRenderDevice");

    return _GetTRenderDevice();

}

SceneManager* TRenderer::GetSceneManager()
{
    GetSceneManagerFunc _GetSceneManager = 0;

    _GetSceneManager = (GetSceneManagerFunc) GetProcAddress(m_hDLL, "GetSceneManager");

    return _GetSceneManager();

}

ShaderManager* TRenderer::GetShaderManager()
{
    GetShaderManagerFunc _GetShaderManager = 0;

    _GetShaderManager = (GetShaderManagerFunc)GetProcAddress(m_hDLL, "GetShaderManager");

    return _GetShaderManager();

}

RenderPipeline* TRenderer::GetRenderPipeline()
{
    GetRenderPipelineFunc _GetRenderPipeline = 0;

    _GetRenderPipeline = (GetRenderPipelineFunc)GetProcAddress(m_hDLL, "GetRenderPipeline");

    return _GetRenderPipeline();

}
