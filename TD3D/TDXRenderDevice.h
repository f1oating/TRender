#ifndef TDXRENDERDEVICE_H
#define TDXRENDERDEVICE_H

#include "TRenderDevice.h"
#include <d3d11.h>
#include <wrl/client.h>

class TDXRenderDevice : TRenderDevice
{
public:
	TDXRenderDevice();

	virtual bool Initizialize(HWND hWnd, int width, int height) override;

	virtual void BeginFrame(float r, float g, float b, float a) override;
	virtual void EndFrame() override;

	virtual void Draw(TVertexColor* vertices, unsigned short numVertices) override;
	virtual void DrawIndexed(TVertexColor* vertices, unsigned short numVertices, unsigned short* indices, unsigned short numIndices) override;

	virtual void SetProjectionMatrix(float width, float height, float farZ, float nearZ) override;

	virtual bool OnResize(int width, int height) override;

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;

	Microsoft::WRL::ComPtr <ID3D11VertexShader> m_pVertexShader;
	Microsoft::WRL::ComPtr <ID3D11PixelShader> m_pPixelShader;
	Microsoft::WRL::ComPtr <ID3DBlob> m_pBlob;
	Microsoft::WRL::ComPtr <ID3D11InputLayout> m_pInputLayout;

};

#endif
