#ifndef TRENDERDEVICE_H
#define TRENDERDEVICE_H

#include <windows.h>
#include <string>
#include "T.h" 
#include "Eigen/Dense"

class TRenderDevice
{
public:
	virtual ~TRenderDevice() {};

	virtual bool Initizialize(HWND hWnd, int width, int height) = 0;

	virtual void BeginFrame(float r, float g, float b, float a) = 0;
	virtual void EndFrame() = 0;

	virtual void Draw(unsigned short numIndices, unsigned short startIndexLocation, unsigned short baseVertexLocation) = 0;

	virtual void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ) = 0;
	virtual void SetViewMatrix(const Eigen::Matrix4d& matrix) = 0;

	virtual void AddTexture(std::string name, std::string path) = 0;
	virtual void AddCubeMapTexture(std::string name, std::string path, std::string ext) = 0;
	virtual void BindTexture(std::string name) = 0;

	virtual void BindVertexShader(std::string name) = 0;
	virtual void BindPixelShader(std::string name) = 0;

	virtual void CreateStaticVertexBuffer(std::string name, void* vertices, unsigned short numVertices, unsigned short vertexSize) = 0;
	virtual void CreateStaticIndexBuffer(std::string name, unsigned short* indices, unsigned short numIndices) = 0;
	virtual void UpdateStaticVertexBuffer(std::string name, void* vertices) = 0;
	virtual void UpdateStaticIndexBuffer(std::string name, unsigned short* indices) = 0;
	virtual void BindVertexBuffer(std::string vertexName, UINT stride, UINT offset) = 0;
	virtual void BindIndexBuffer(std::string indexName) = 0;
	virtual void DeleteBuffer(std::string name) = 0;

	virtual void SetDepthStencilComparison(bool flag) = 0;
	virtual void SetRasterizerCulling(bool flag) = 0;

	virtual bool OnResize(int width, int height) = 0;
	virtual bool IsRunning() = 0;

protected:
	bool m_IsRunning;

};

extern "C" {
	HRESULT CreateRenderDevice(TRenderDevice** pInterface);
	typedef HRESULT(*CREATERENDERDEVICE)(TRenderDevice** pInterface);

	HRESULT ReleaseRenderDevice(TRenderDevice** pInterface);
	typedef HRESULT(*RELEASERENDERDEVICE)(TRenderDevice** pInterface);
}

#endif
