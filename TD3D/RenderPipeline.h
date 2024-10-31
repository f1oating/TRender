#ifndef RENDERPIPELINE_H
#define RENDERPIPELINE_H

#include <memory>
#include <vector>

#include "RenderCommand.h"
#include "TRenderDevice.h"

class RenderPipeline {
public:
    RenderPipeline(TRenderDevice* renderDevice);

    void AddRenderCommand(std::shared_ptr<RenderCommand> command);
    void Execute(ID3D11DeviceContext* context);

    void ClearPipeline();

private:
    std::vector<std::shared_ptr<RenderCommand>> renderCommands;
    TRenderDevice* renderDevice;
};

#endif // RENDERPIPELINE_H
