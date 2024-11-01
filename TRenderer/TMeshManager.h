#ifndef TMESHMANAGER_H
#define TMESHMANAGER_H

#include "T.h"
#include "TMesh.h"
#include <string>
#include <unordered_map>

class TMeshManager
{
public:
	virtual ~TMeshManager() {};

	virtual TMesh* AddMesh(TVertexColor* vertices, unsigned short numVertices, std::string key) = 0;
	virtual TMesh* AddMesh(TVertexColor* vertices, unsigned short numVertices, unsigned short* indices, unsigned short numIndices, std::string key) = 0;
	virtual TMesh* GetMesh(std::string key) = 0;
	virtual bool RemoveMesh(std::string key) = 0;

	virtual void Flush() = 0;

};

#endif
