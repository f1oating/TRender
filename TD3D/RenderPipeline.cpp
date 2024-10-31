#include "RenderPipeline.h"
#include "RenderCommand.h"
#include <stdexcept>

RenderPipeline RenderPipeline::m_RenderPipeline;

RenderPipeline::RenderPipeline()
{

}

void RenderPipeline::AddRenderCommand(std::shared_ptr<RenderCommand> command) {
    if (!command) {
        throw std::invalid_argument("RenderCommand cannot be null");
    }
    renderCommands.push_back(command);
}

void RenderPipeline::Execute(ID3D11DeviceContext* context) {
    if (!context) {
        throw std::invalid_argument("Device context cannot be null");
    }

    for (const auto& command : renderCommands) {
        command->Execute(context);
    }
}

void RenderPipeline::ClearPipeline() {
    renderCommands.clear();
}

RenderPipeline* RenderPipeline::GetRenderPipeline()
{
    return &m_RenderPipeline;
}

RenderPipeline* GetRenderPipeline()
{
    return RenderPipeline::GetRenderPipeline();
}
