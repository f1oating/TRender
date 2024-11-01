#ifndef TDXMESHMANAGER_H
#define TDXMESHMANAGER_H

#include "TMeshManager.h"
#include "TDXRenderDevice.h"
#include <unordered_map>

class TDXMeshManager : public TMeshManager
{
public:
	TDXMeshManager(TDXRenderDevice* tdxRenderDevice);

	virtual TMesh* AddMesh(TVertexColor* vertices, unsigned short numVertices, std::string key) override;
	virtual TMesh* AddMesh(TVertexColor* vertices, unsigned short numVertices, unsigned short* indices, unsigned short numIndices, std::string key) override;
	virtual TMesh* GetMesh(std::string key) override;
	virtual bool RemoveMesh(std::string key) override;

	virtual void Flush() override;

private:
	std::unordered_map<std::string, TMesh*> m_MeshesMap;
	TDXRenderDevice* m_TDXRenderDevice;

};

#endif
