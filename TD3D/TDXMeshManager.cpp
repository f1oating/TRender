#include "TDXMeshManager.h"
#include "TDXMesh.h"

TDXMeshManager::TDXMeshManager(TDXRenderDevice* tdxRenderDevice)
{
	this->tdxRenderDevice = tdxRenderDevice;
}

TMesh* TDXMeshManager::AddMesh(TVertexColor* vertices, unsigned short numVertices, std::string key)
{
	if (m_MeshesMap.find(key) != m_MeshesMap.end())
	{
		return m_MeshesMap[key];
	}
	else
	{
		TMesh* mesh = new TDXMesh();
		mesh->CreateBuffers(vertices, numVertices, tdxRenderDevice);
		m_MeshesMap[key] = mesh;
		return mesh;
	}
}

TMesh* TDXMeshManager::AddMesh(TVertexColor* vertices, unsigned short numVertices, unsigned short* indices, unsigned short numIndices, std::string key)
{
	if (m_MeshesMap.find(key) != m_MeshesMap.end())
	{
		return m_MeshesMap[key];
	}
	else
	{
		TMesh* mesh = new TDXMesh();
		mesh->CreateBuffers(vertices, numVertices, indices, numIndices, tdxRenderDevice);
		m_MeshesMap[key] = mesh;
		return mesh;
	}
}

TMesh* TDXMeshManager::GetMesh(std::string key)
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

bool TDXMeshManager::RemoveMesh(std::string key)
{
	return m_MeshesMap.erase(key);
}

void TDXMeshManager::Flush()
{
	m_MeshesMap.clear();
}
