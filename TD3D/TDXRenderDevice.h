#ifndef TDXRENDERDEVICE_H
#define TDXRENDERDEVICE_H

#include "TRenderDevice.h"
#include "TDXShaderManager.h"
#include "TDXTextureManager.h"
#include "TDXFeatureController.h"
#include "TDXBufferManager.h"
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

	virtual void Draw(unsigned short numIndices, unsigned short startIndexLocation, unsigned short baseVertexLocation) override;

	virtual void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ) override;
	virtual void SetViewMatrix(Eigen::Matrix4d matrix) override;

	virtual void AddTexture(std::string name, std::string path) override;
	virtual void AddCubeMapTexture(std::string name, std::string path, std::string ext) override;
	virtual void BindTexture(std::string name) override;

	virtual void BindVertexShader(std::string name) override;
	virtual void BindPixelShader(std::string name) override;

	virtual void CreateStaticVertexBuffer(std::string name, void* vertices, unsigned short numVertices, unsigned short vertexSize) override;
	virtual void CreateStaticIndexBuffer(std::string name, unsigned short* indices, unsigned short numIndices) override;
	virtual void UpdateStaticVertexBuffer(std::string name, void* vertices) override;
	virtual void UpdateStaticIndexBuffer(std::string name, unsigned short* indices) override;
	virtual void BindVertexBuffer(std::string vertexName, UINT stride, UINT offset) override;
	virtual void BindIndexBuffer(std::string indexName) override;
	virtual void DeleteBuffer(std::string name) override;

	virtual void SetDepthStencilComparison(bool flag) override;
	virtual void SetRasterizerCulling(bool flag) override;

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

	DirectX::XMMATRIX m_ViewMatrix;
	DirectX::XMMATRIX m_ProjectionMatrix;

	Microsoft::WRL::ComPtr <ID3D11Buffer> m_pViewProjectionBuffer;

	ViewProjectionConstantBuffer m_ViewProjectionConstantBuffer;

	TDXShaderManager m_TDXShaderManager;
	TDXTextureManager m_TDXTextureManager;
	TDXFeatureController m_TDXFeatureController;
	TDXBufferManager m_TDXBufferManager;

};

#endif
