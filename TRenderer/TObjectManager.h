#ifndef TObjectManager_H
#define TObjectManager_H

#include "T.h"
#include "TMesh.h"
#include <string>
#include <unordered_map>

class TObjectManager
{
public:
	virtual ~TObjectManager() {};

	virtual TMesh* AddMesh(TVertexColor* vertices, unsigned short numVertices, std::string key) = 0;
	virtual TMesh* AddMesh(TVertexColor* vertices, unsigned short numVertices, unsigned short* indices, unsigned short numIndices, std::string key) = 0;
	virtual TMesh* GetMesh(std::string key) = 0;
	virtual bool RemoveMesh(std::string key) = 0;

	virtual void FlushMeshes() = 0;

};

#endif
