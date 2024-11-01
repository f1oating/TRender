#ifndef TDXMESH_H
#define TDXMESH_H

#include "TMesh.h"
#include <d3d11.h>

class TDXMesh : public TMesh
{
public:
	virtual void CreateBuffers(TVertexColor* vertices, unsigned short numVertices, TRenderDevice* tRenderDevice) override;
	virtual void CreateBuffers(TVertexColor* vertices, unsigned short numVertices, unsigned short* indices, unsigned
		short numIndices, TRenderDevice* tRenderDevice) override;

	virtual void Render(TRenderDevice* tRenderDevice) override;

private:
	UINT numIndices;
	UINT numVertices;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;

};

#endif
