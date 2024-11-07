#ifndef TDXRENDERDEVICE_H
#define TDXRENDERDEVICE_H

#include "TRenderDevice.h"
#include "TDXShaderManager.h"
#include "TDXTextureManager.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

struct TransformConstantBuffer
{
	DirectX::XMMATRIX transform;
};

class TDXRenderDevice : public TRenderDevice
{
public:
	TDXRenderDevice();
	~TDXRenderDevice();

	virtual bool Initizialize(HWND hWnd, int width, int height) override;

	virtual void BeginFrame(float r, float g, float b, float a) override;
	virtual void EndFrame() override;

	virtual void Draw(TVertexPT* vertices, unsigned short numVertices) override;
	virtual void Draw(TVertexPT* vertices, unsigned short numVertices, unsigned short* indices, unsigned short numIndices) override;

	virtual void SetProjectionMatrix(float fieldOfView, float aspectRatio, float nearZ, float farZ) override;
	virtual void SetViewMatrix(TVector4 eye, TVector4 at, TVector4 up) override;

	virtual void AddTexture(std::string name, std::string path) override;
	virtual void BindTexture(std::string name) override;

	virtual bool OnResize(int width, int height) override;
	virtual bool IsRunning() override;

	void CreateBuffers();
	void AddShaders();

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;

	Microsoft::WRL::ComPtr <ID3D11Buffer> m_pTransformBuffer;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;

	DirectX::XMMATRIX m_ProjMatrix;
	DirectX::XMMATRIX m_ViewMatrix;

	TransformConstantBuffer m_TransformConstantBuffer;

	TDXShaderManager m_TDXShaderManager;
	TDXTextureManager m_TDXTextureManager;

};

#endif
