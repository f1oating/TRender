#ifndef RENDER_COMMAND_H
#define RENDER_COMMAND_H

#include <memory>

class Model;
class ID3D11DeviceContext;

class RenderCommand {
public:
    RenderCommand(std::shared_ptr<Model> model);
    void Execute(ID3D11DeviceContext* context);

private:
    std::shared_ptr<Model> model;
};

#endif // RENDER_COMMAND_H
