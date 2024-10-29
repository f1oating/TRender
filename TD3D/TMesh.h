#ifndef TMESH_H
#define TMESH_H

#include <d3d11.h>

#include "T.h"

class TMesh
{
public:
	TMesh(TVertex* vertices, size_t vertexCount, UINT* indices, size_t indexCount);
	~TMesh();

	void Render();

private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	unsigned int indicesToRender;

};

#endif TMESH_H
