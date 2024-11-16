#ifndef TRENDERDEVICE_H
#define TRENDERDEVICE_H

#include <windows.h>
#include <string>
#include "T.h" 

class TRenderDevice
{
public:
	virtual ~TRenderDevice() {};

	virtual bool Initizialize(HWND hWnd, int width, int height) = 0;

	virtual void BeginFrame(float r, float g, float b, float a) = 0;
	virtual void EndFrame() = 0;

	virtual void DrawPT(unsigned short numIndices, unsigned short startIndexLocation, unsigned short baseVertexLocation) = 0;

	virtual void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ) = 0;

	virtual void SetViewPosition(float x, float y, float z) = 0;
	virtual void AdjustPosition(float x, float y, float z) = 0;
	virtual void SetRotation(float x, float y, float z) = 0;
	virtual void AdjustRotation(float x, float y, float z) = 0;
	virtual void SetLookAtPos(float x, float y, float z) = 0;

	virtual void UpdatePTBuffer(TVertexPT* vertices, unsigned short numVertices, unsigned short* indices, unsigned short numIndices) = 0;

	virtual void AddTexture(std::string name, std::string path) = 0;
	virtual void AddCubeMapTexture(std::string name, std::string path, std::string ext) = 0;
	virtual void BindTexture(std::string name) = 0;

	virtual void BindVertexShader(std::string name) = 0;
	virtual void BindPixelShader(std::string name) = 0;

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
