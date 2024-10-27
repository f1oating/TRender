#include "TMesh.h"

#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <list>

TMesh::TMesh(ID3D11Device* device, TVertex sverticies[], UINT sindicies[]) {
    HRESULT hr;

    TVertex vertices[] =
    {
        { 0.0f,0.5f,255,0,0,0 },
        { 0.5f,-0.5f,0,255,0,0 },
        { -0.5f,-0.5f,0,0,255,0 },
        { -0.3f,0.3f,0,255,0,0 },
        { 0.3f,0.3f,0,0,255,0 },
        { 0.0f,-0.8f,255,0,0,0 },
    };

    const unsigned short indices[] =
    {
        0,1,2,
        0,2,3,
        0,4,1,
        2,1,5,
    };

    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = sizeof(vertices);
    bd.StructureByteStride = sizeof(TVertex);

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;
    hr =device->CreateBuffer(&bd, &sd, &vertexBuffer);

    D3D11_BUFFER_DESC ibd = {};
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.CPUAccessFlags = 0u;
    ibd.MiscFlags = 0u;
    ibd.ByteWidth = sizeof(indices);
    ibd.StructureByteStride = sizeof(unsigned short);
    D3D11_SUBRESOURCE_DATA isd = {};
    isd.pSysMem = indices;
    hr = device->CreateBuffer(&ibd, &isd, &indexBuffer);

    indexCount = sizeof(indices);
}

TMesh::~TMesh() {
    if (vertexBuffer) vertexBuffer->Release();
    if (indexBuffer) indexBuffer->Release();
}

void TMesh::Render(ID3D11DeviceContext* context, ID3D11Device* device) {
    HRESULT hr;

    const UINT stride = sizeof(TVertex);
    const UINT offset = 0u;

    context->IASetVertexBuffers(0, 1u, &vertexBuffer, &stride, &offset);
    context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0u);

    ID3D11PixelShader* pPixelShader;
    ID3DBlob* pBlob;
    D3DReadFileToBlob(L"C:\\Users\\Alan\\Desktop\\game-dev\\TEngine\\TRender\\TD3D\\PixelShader.cso", &pBlob);
    hr = device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

    // bind pixel shader
    context->PSSetShader(pPixelShader, nullptr, 0u);


    // create vertex shader
    ID3D11VertexShader* pVertexShader;
    D3DReadFileToBlob(L"C:\\Users\\Alan\\Desktop\\game-dev\\TEngine\\TRender\\TD3D\\VertexShader.cso", &pBlob);
    hr = device->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);

    // bind vertex shader
    context->VSSetShader(pVertexShader, nullptr, 0u);


    // input (vertex) layout (2d position only)
    ID3D11InputLayout* pInputLayout;
    const D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        { "Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
        { "Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,8u,D3D11_INPUT_PER_VERTEX_DATA,0 },
    };
    hr = device->CreateInputLayout(
        ied, (UINT)std::size(ied),
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        &pInputLayout
    );

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // bind vertex layout
    context->IASetInputLayout(pInputLayout);

    context->DrawIndexed(indexCount, 0, 0);
}
