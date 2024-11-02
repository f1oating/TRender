#include "TDXObjectManager.h"
#include "TDXMesh.h"
#include "TDXPointLight.h"

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
		TDXMesh* mesh = new TDXMesh(vertices, numVertices, m_TDXRenderDevice);
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
		TDXMesh* mesh = new TDXMesh(vertices, numVertices, indices, numIndices, m_TDXRenderDevice);
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

TPointLight* TDXObjectManager::AddPointLight(TVector3 position, TVector3 color, float range, float intencity, std::string key)
{
	if (m_PointLightsMap.size() > 7)
	{
		return nullptr;
	}

	if (m_PointLightsMap.find(key) != m_PointLightsMap.end())
	{
		return m_PointLightsMap[key];
	}
	else
	{
		TDXPointLight* pointLight = new TDXPointLight(position, color, range, intencity);
		m_PointLightsMap[key] = pointLight;
		return pointLight;
	}
}

TPointLight* TDXObjectManager::GetPointLight(std::string key)
{
	if (m_PointLightsMap.find(key) != m_PointLightsMap.end())
	{
		return m_PointLightsMap[key];
	}
	else
	{
		return nullptr;
	}
}

bool TDXObjectManager::RemovePointLight(std::string key)
{
	if (m_PointLightsMap.find(key) != m_PointLightsMap.end())
	{
		delete m_PointLightsMap[key];
		return m_PointLightsMap.erase(key);
	}
	else
	{
		return 0;
	}
}

void TDXObjectManager::FlushPointLights()
{
	for (const std::pair<std::string, TPointLight*> pair : m_PointLightsMap) {
		delete pair.second;
	}
	m_PointLightsMap.clear();
}

void TDXObjectManager::CreatePointLightConstantBuffer()
{
	D3D11_BUFFER_DESC pointLightBufferDesc = {};
	pointLightBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	pointLightBufferDesc.ByteWidth = sizeof(PointLightConstantBuffer);
	pointLightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pointLightBufferDesc.CPUAccessFlags = 0;
	HRESULT hr = m_TDXRenderDevice->m_pDevice->CreateBuffer(&pointLightBufferDesc, nullptr, &m_pPointLightConstantBuffer);
}

void TDXObjectManager::UpdatePointLightConstantBuffer()
{
	if (!m_PointLightsMap.empty())
	{
		PointLightConstantBuffer cb;
		int count = 0;

		for (const std::pair<std::string, TPointLight*> pair : m_PointLightsMap)
		{
			TVector3 position = ((TDXPointLight*)pair.second)->m_Position;
			TVector3 color = ((TDXPointLight*)pair.second)->m_Color;
			float range = ((TDXPointLight*)pair.second)->m_Range;
			float intencity = ((TDXPointLight*)pair.second)->m_Intencity;

			PointLight pl = {
				{position.x, position.y, position.z},
				range,
				{color.x, color.y, color.z},
				intencity
			};
			cb.pointLight[count] = pl;
			count++;
		}

		cb.numPointLights = count;

		m_TDXRenderDevice->m_pDeviceContext->UpdateSubresource(m_pPointLightConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);
		m_TDXRenderDevice->m_pDeviceContext->PSSetConstantBuffers(1, 1, m_pPointLightConstantBuffer.GetAddressOf());
	}
}
