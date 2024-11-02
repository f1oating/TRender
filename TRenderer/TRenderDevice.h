#ifndef TRENDERDEVICE_H
#define TRENDERDEVICE_H

#include <windows.h>
#include "T.h" 

class TObjectManager;

class TRenderDevice
{
public:
	virtual ~TRenderDevice() {};

	virtual bool Initizialize(HWND hWnd, int width, int height) = 0;

	virtual void BeginFrame(float r, float g, float b, float a) = 0;
	virtual void EndFrame() = 0;

	virtual void Draw(TVertexColor* vertices, unsigned short numVertices) = 0;
	virtual void Draw(TVertexColor* vertices, unsigned short numVertices, unsigned short* indices, unsigned short numIndices) = 0;

	virtual void SetProjectionMatrix(float fieldOfView, float aspectRatio, float nearZ, float farZ) = 0;
	virtual void SetViewMatrix(TVector4 eye, TVector4 at, TVector4 up) = 0;

	virtual void SetAmbientLight(float r, float g, float b, float a) = 0;

	virtual TObjectManager* GeTObjectManager() = 0;

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
