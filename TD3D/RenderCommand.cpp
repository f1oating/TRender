#include "RenderCommand.h"
#include "Model.h"
#include <d3d11.h>

RenderCommand::RenderCommand(std::shared_ptr<Model> model)
    : model(model) {}

void RenderCommand::Execute(ID3D11DeviceContext* context) {
    if (model) {
        model->Render(context);
    }
}
