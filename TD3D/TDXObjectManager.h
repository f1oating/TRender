#ifndef TDXObjectManager_H
#define TDXObjectManager_H

#include "TObjectManager.h"
#include "TDXRenderDevice.h"
#include <unordered_map>

class TDXObjectManager : public TObjectManager
{
public:
	TDXObjectManager(TDXRenderDevice* tdxRenderDevice);
	~TDXObjectManager();

	virtual TMesh* AddMesh(TVertexColor* vertices, unsigned short numVertices, std::string key) override;
	virtual TMesh* AddMesh(TVertexColor* vertices, unsigned short numVertices, unsigned short* indices, unsigned short numIndices, std::string key) override;
	virtual TMesh* GetMesh(std::string key) override;
	virtual bool RemoveMesh(std::string key) override;

	virtual void FlushMeshes() override;

private:
	std::unordered_map<std::string, TMesh*> m_MeshesMap;
	TDXRenderDevice* m_TDXRenderDevice;

};

#endif
