#ifndef TDXMESH_H
#define TDXMESH_H

#include "TMesh.h"
#include "T.h"
#include <d3d11.h>
#include <DirectXMath.h>

struct TConstantBuffer
{
	DirectX::XMMATRIX mvpMatrix;
};

class TDXMesh : public TMesh
{
public:
	TDXMesh();

	virtual void CreateBuffers(TVertexColor* vertices, unsigned short numVertices, TRenderDevice* tRenderDevice) override;
	virtual void CreateBuffers(TVertexColor* vertices, unsigned short numVertices, unsigned short* indices, unsigned
		short numIndices, TRenderDevice* tRenderDevice) override;

	virtual void SetPosition(float x, float y, float z) override;

	virtual void SetRotationX(float angle) override;
	virtual void SetRotationY(float angle) override;
	virtual void SetRotationZ(float angle) override;

	virtual void Render(TRenderDevice* tRenderDevice) override;

private:
	UINT m_NumIndices;
	UINT m_NumVertices;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;

	TPosition m_Pos;

	float m_RotationX;
	float m_RotationY;
	float m_RotationZ;

};

#endif
