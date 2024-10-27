#ifndef TMESH_H
#define TMESH_H

#include <d3d11.h>

#include "T.h"

class TMesh
{
public:
	TMesh(ID3D11Device* device, TVertex verticies[], UINT indicies[]);
	~TMesh();

	void Render(ID3D11DeviceContext* context, ID3D11Device* device);

private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	unsigned int indexCount;

};

#endif TMESH_H
