#ifndef TRENDERER_H
#define TRENDERER_H

#include <windows.h>
#include "TRenderDevice.h"
#include "SceneManager.h"
#include "ShaderManager.h"
#include "RenderPipeline.h"

class TRenderer 
{
public:
    TRenderer(HINSTANCE hInst);

    TRenderDevice* GetRenderDevice();
    SceneManager* GetSceneManager();
    ShaderManager* GetShaderManager();
    RenderPipeline* GetRenderPipeline();


    HINSTANCE GetModule() const { return m_hDLL; }

private:
    HINSTANCE m_hInst;
    HMODULE m_hDLL;
};

#endif // TRENDERER_H
