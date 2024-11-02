#include "TDXObjectManager.h"
#include "TDXMesh.h"

TDXObjectManager::TDXObjectManager(TDXRenderDevice* tdxRenderDevice)
{
	m_TDXRenderDevice = tdxRenderDevice;
}

TDXObjectManager::~TDXObjectManager()
{
	FlushMeshes();
}

TMesh* TDXObjectManager::AddMesh(TVertexColor* vertices, unsigned short numVertices, std::string key)
{
	if (m_MeshesMap.find(key) != m_MeshesMap.end())
	{
		return m_MeshesMap[key];
	}
	else
	{
		TMesh* mesh = new TDXMesh();
		mesh->CreateBuffers(vertices, numVertices, m_TDXRenderDevice);
		m_MeshesMap[key] = mesh;
		return mesh;
	}
}

TMesh* TDXObjectManager::AddMesh(TVertexColor* vertices, unsigned short numVertices, unsigned short* indices, unsigned short numIndices, std::string key)
{
	if (m_MeshesMap.find(key) != m_MeshesMap.end())
	{
		return m_MeshesMap[key];
	}
	else
	{
		TMesh* mesh = new TDXMesh();
		mesh->CreateBuffers(vertices, numVertices, indices, numIndices, m_TDXRenderDevice);
		m_MeshesMap[key] = mesh;
		return mesh;
	}
}

TMesh* TDXObjectManager::GetMesh(std::string key)
{
	if (m_MeshesMap.find(key) != m_MeshesMap.end())
	{
		return m_MeshesMap[key];
	}
	else
	{
		return nullptr;
	}
}

bool TDXObjectManager::RemoveMesh(std::string key)
{
	if (m_MeshesMap.find(key) != m_MeshesMap.end())
	{
		delete m_MeshesMap[key];
		return m_MeshesMap.erase(key);
	}
	else
	{
		return 0;
	}
}

void TDXObjectManager::FlushMeshes()
{
	for (const std::pair<std::string, TMesh*> pair : m_MeshesMap) {
		delete pair.second;
	}
	m_MeshesMap.clear();
}
