#include "TD3D.h"

#include <stdexcept>
#include <d3dcompiler.h>
#include <wrl.h>

void TD3D::BeginRendering()
{
    m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);
    m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    float color[4] = { 0.5f, 0.2f, 0.4f, 1.0f };
    m_pContext->ClearRenderTargetView(m_pRenderTargetView, color);

	//DrawTestTriangle();

    for (TMesh* mesh : m_pMeshes)
    {
        mesh->Render(m_pContext, m_pDevice);
    }
}

void TD3D::EndRendering()
{
    m_pSwapChain->Present(1, 0);
}

void TD3D::LoadMesh(TVertex verticies[], UINT indicies[])
{
    TMesh* mesh = new TMesh(m_pDevice, verticies, indicies);
    m_pMeshes.push_back(mesh);
}

bool TD3D::IsRunning() const
{
    return m_isRunning;
}

void TD3D::DrawTestTriangle()
{
	HRESULT hr;

	namespace wrl = Microsoft::WRL;

	struct Vertex
	{
		struct
		{
			float x;
			float y;
		} pos;
		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} color;
	};

	// create vertex buffer (1 2d triangle at center of screen)
	Vertex vertices[] =
	{
		{ 0.0f,0.5f,255,0,0,0 },
		{ 0.5f,-0.5f,0,255,0,0 },
		{ -0.5f,-0.5f,0,0,255,0 },
		{ -0.3f,0.3f,0,255,0,0 },
		{ 0.3f,0.3f,0,0,255,0 },
		{ 0.0f,-0.8f,255,0,0,0 },
	};
	vertices[0].color.g = 255;
	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	m_pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);

	// Bind vertex buffer to pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	m_pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);


	// create index buffer
	const unsigned short indices[] =
	{
		0,1,2,
		0,2,3,
		0,4,1,
		2,1,5,
	};
	wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	m_pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer);

	// bind index buffer
	m_pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);


	// create pixel shader
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	wrl::ComPtr<ID3DBlob> pBlob;
	hr = D3DReadFileToBlob(L"C:\\Users\\Alan\\Desktop\\game-dev\\TEngine\\TRender\\TD3D\\PixelShader.cso", &pBlob);
	m_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

	// bind pixel shader
	m_pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);


	// create vertex shader
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	hr = D3DReadFileToBlob(L"C:\\Users\\Alan\\Desktop\\game-dev\\TEngine\\TRender\\TD3D\\VertexShader.cso", &pBlob);
	m_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);

	// bind vertex shader
	m_pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);


	// input (vertex) layout (2d position only)
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,8u,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	m_pDevice->CreateInputLayout(
		ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	);

	// bind vertex layout
	m_pContext->IASetInputLayout(pInputLayout.Get());


	// bind render target
	m_pContext->OMSetRenderTargets(1u, &m_pRenderTargetView, nullptr);


	// Set primitive topology to triangle list (groups of 3 vertices)
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	// configure viewport
	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pContext->RSSetViewports(1u, &vp);


	m_pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u);
}

