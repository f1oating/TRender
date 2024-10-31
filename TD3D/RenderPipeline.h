#ifndef RENDERPIPELINE_H
#define RENDERPIPELINE_H

#include <memory>
#include <vector>

#include "RenderCommand.h"

class RenderPipeline {
protected:
    RenderPipeline();
    static RenderPipeline m_RenderPipeline;

public:

    void AddRenderCommand(std::shared_ptr<RenderCommand> command);
    void Execute(ID3D11DeviceContext* context);

    void ClearPipeline();

    static RenderPipeline* GetRenderPipeline();

private:
    std::vector<std::shared_ptr<RenderCommand>> renderCommands;

};

extern "C"
{
    RenderPipeline* GetRenderPipeline();
    typedef RenderPipeline* (*GetRenderPipelineFunc)();
}

#endif // RENDERPIPELINE_H
