#ifndef TDXRENDERDEVICE_H
#define TDXRENDERDEVICE_H

#include "TRenderDevice.h"
#include "TDXShaderManager.h"
#include "TDXTextureManager.h"
#include "TDXFeatureController.h"
#include "TD3D.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

struct ViewProjectionConstantBuffer
{
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};

class TDXRenderDevice : public TRenderDevice
{
public:
	TDXRenderDevice();
	~TDXRenderDevice();

	virtual bool Initizialize(HWND hWnd, int width, int height) override;

	virtual void BeginFrame(float r, float g, float b, float a) override;
	virtual void EndFrame() override;

	virtual void DrawPT(unsigned short numIndices, unsigned short startIndexLocation, unsigned short baseVertexLocation) override;

	virtual void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ) override;

	virtual void SetViewPosition(float x, float y, float z) override;
	virtual void AdjustPosition(float x, float y, float z) override;
	virtual void SetRotation(float x, float y, float z) override;
	virtual void AdjustRotation(float x, float y, float z) override;
	virtual void SetLookAtPos(float x, float y, float z) override;

	virtual void UpdatePTBuffer(TVertexPT* vertices, unsigned short numVertices, unsigned short* indices, unsigned short numIndices) override;

	virtual void AddTexture(std::string name, std::string path) override;
	virtual void BindTexture(std::string name) override;

	virtual void BindVertexShader(std::string name) override;
	virtual void BindPixelShader(std::string name) override;

	virtual void SetDepthStencilComparison(bool flag) override;
	virtual void SetRasterizerCulling(bool flag) override;

	virtual bool OnResize(int width, int height) override;
	virtual bool IsRunning() override;

	void CreateBuffers();
	void AddShaders();
	void UpdateViewMatrix();

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;

	Microsoft::WRL::ComPtr <ID3D11Buffer> m_pViewProjectionBuffer;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pStaticVertexBufferPT;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pStaticIndexBufferPT;

	ViewProjectionConstantBuffer m_ViewProjectionConstantBuffer;

	TDXShaderManager m_TDXShaderManager;
	TDXTextureManager m_TDXTextureManager;
	TDXFeatureController m_TDXFeatureController;

	TDXCamera m_Camera;

};

#endif
