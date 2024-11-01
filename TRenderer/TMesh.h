#ifndef TMESH_H
#define TMESH_H

#include "T.h"
#include "TRenderDevice.h"
#include <wrl/client.h>
#include <d3d11.h>

class TMesh
{
public:
	virtual ~TMesh() {};

	virtual void CreateBuffers(TVertexColor* vertices, unsigned short numVertices, TRenderDevice* tRenderDevice) = 0;
	virtual void CreateBuffers(TVertexColor* vertices, unsigned short numVertices, unsigned short* indices, unsigned
		short numIndices, TRenderDevice* tRenderDevice) = 0;

	virtual void SetPosition(float x, float y, float z) = 0;

	virtual void SetRotationX(float angle) = 0;
	virtual void SetRotationY(float angle) = 0;
	virtual void SetRotationZ(float angle) = 0;

	virtual void Render(TRenderDevice* tRenderDevice) = 0;

};

#endif