#ifndef TRENDERDEVICE_H
#define TRENDERDEVICE_H

#include <windows.h>
#include "T.h" 

class TRenderDevice
{
public:
	virtual bool Initizialize(HWND hWnd, int width, int height);

	virtual void BeginFrame(float r, float g, float b, float a);
	virtual void EndFrame();

	virtual void Draw(TVertex3* vertices, unsigned short numVertices);
	virtual void DrawIndexed(TVertex3* vertices, unsigned short numVertices, unsigned short* indices, unsigned short numIndices);

	virtual void SetProjectionMatrix(float width, float height, float farZ, float nearZ);

	virtual bool OnResize(int width, int height);

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
