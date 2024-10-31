#ifndef TRENDERDEVICE_H
#define TRENDERDEVICE_H

#include <windows.h>
#include "T.h" 

class TRenderDevice
{
public:
	virtual ~TRenderDevice() {};

	virtual bool Initizialize(HWND hWnd, int width, int height) = 0;

	virtual void BeginFrame(float r, float g, float b, float a) = 0;
	virtual void EndFrame() = 0;

	virtual void Draw(TVertexColor* vertices, unsigned short numVertices) = 0;
	virtual void DrawIndexed(TVertexColor* vertices, unsigned short numVertices, unsigned short* indices, unsigned short numIndices) = 0;

	virtual void SetProjectionMatrix(float width, float height, float farZ, float nearZ) = 0;

	virtual bool OnResize(int width, int height) = 0;

protected:
	HWND hWnd;

};

extern "C" {
	HRESULT CreateRenderDevice(TRenderDevice** pInterface);
	typedef HRESULT(*CREATERENDERDEVICE)(TRenderDevice** pInterface);

	HRESULT ReleaseRenderDevice(TRenderDevice** pInterface);
	typedef HRESULT(*RELEASERENDERDEVICE)(TRenderDevice** pInterface);
}

#endif
