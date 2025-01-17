#ifndef TRENDERDEVICE_H
#define TRENDERDEVICE_H

#include <windows.h>
#include <string>
#include <vector>
#include "T.h" 
#include "Eigen/Dense"

class TRenderDevice
{
public:
	virtual ~TRenderDevice() {};

	virtual bool Initizialize(HWND hWnd, int width, int height) = 0;

	virtual void BeginShadowPass() = 0;

	virtual void BeginFrame(float r, float g, float b, float a) = 0;
	virtual void EndFrame() = 0;

	virtual void Draw(unsigned int numIndices, unsigned int startIndexLocation, unsigned int baseVertexLocation) = 0;
	virtual void DrawSprite(unsigned int numVertices, unsigned int startVertexLocation) = 0;
	virtual void RenderText(const wchar_t* text, float x, float y) = 0;

	virtual void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ) = 0;
	virtual void SetViewMatrix(const Eigen::Matrix4f& matrix) = 0;
	virtual void SetWorldMatrix(const Eigen::Matrix4f& matrix) = 0;

	virtual void AddTexture(std::string name, std::string path) = 0;
	virtual void AddTexture(std::string name, unsigned char* data, int width, int height) = 0;
	virtual void AddTexture(std::string name, unsigned char* data, size_t dataSize) = 0;
	virtual void AddTexture(std::string name, unsigned int r, unsigned int g, unsigned int b, unsigned int a) = 0;
	virtual void AddCubeMapTexture(std::string name, std::string path, std::string ext) = 0;
	virtual void BindTexture(std::string name) = 0;

	virtual void BindVertexShader(std::string name) = 0;
	virtual void BindPixelShader(std::string name) = 0;
	virtual void BindGeometryShader(std::string name) = 0;

	virtual void UnbindVertexShader(std::string name) = 0;
	virtual void UnbindPixelShader(std::string name) = 0;
	virtual void UnbindGeometryShader(std::string name) = 0;

	virtual void CreateStaticVertexBuffer(std::string name, void* vertices, unsigned int numVertices, unsigned int vertexSize) = 0;
	virtual void CreateStaticIndexBuffer(std::string name, unsigned int* indices, unsigned int numIndices) = 0;
	virtual void CreateDynamicVertexBuffer(std::string name, void* vertices, unsigned int numVertices, unsigned int vertexSize) = 0;
	virtual void CreateDynamicIndexBuffer(std::string name, unsigned int* indices, unsigned int numIndices) = 0;

	virtual void UpdateStaticVertexBuffer(std::string name, void* vertices) = 0;
	virtual void UpdateStaticIndexBuffer(std::string name, unsigned int* indices) = 0;
	virtual void UpdateDynamicVertexBuffer(std::string name, void* vertices, unsigned int numVertices, unsigned int vertexSize) = 0;
	virtual void UpdateDynamicIndexBuffer(std::string name, unsigned int* indices, unsigned int numIndices) = 0;

	virtual void BindVertexBuffer(std::string vertexName, UINT stride, UINT offset) = 0;
	virtual void BindIndexBuffer(std::string indexName) = 0;
	virtual void DeleteBuffer(std::string name) = 0;

	virtual void SetDepthStencilComparison(bool flag) = 0;
	virtual void SetRasterizerCulling(bool flag) = 0;
	virtual void SetBlendState(bool flag) = 0;

	virtual void SetLights(std::vector<Light>& lights) = 0;
	virtual void SetAmbientLight(float r, float g, float b) = 0;
	virtual void SetDirectionalLight(TVector3 direction, TVector3 color, float intensity) = 0;

	virtual bool OnResize(int width, int height) = 0;
	virtual bool IsRunning() = 0;

protected:
	HWND hwnd;
	bool m_IsRunning;

};

extern "C" {
	HRESULT CreateRenderDevice(TRenderDevice** pInterface);
	typedef HRESULT(*CREATERENDERDEVICE)(TRenderDevice** pInterface);

	HRESULT ReleaseRenderDevice(TRenderDevice** pInterface);
	typedef HRESULT(*RELEASERENDERDEVICE)(TRenderDevice** pInterface);
}

#endif
