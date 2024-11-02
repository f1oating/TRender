#ifndef TDXObjectManager_H
#define TDXObjectManager_H

#include "TObjectManager.h"
#include "TDXRenderDevice.h"
#include <unordered_map>

struct PointLight{
	DirectX::XMFLOAT3 Position;
	float Range;
	DirectX::XMFLOAT3 Color;
	float Intensity;
};

struct PointLightConstantBuffer
{
	PointLight pointLight[8];
	int numPointLights;
};

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

	virtual TPointLight* AddPointLight(TVector3 position, TVector3 color, float range, float intencity, std::string key) override;
	virtual TPointLight* GetPointLight(std::string key) override;
	virtual bool RemovePointLight(std::string key) override;
	virtual void FlushPointLights() override;

	void CreatePointLightConstantBuffer();
	void UpdatePointLightConstantBuffer();

private:
	std::unordered_map<std::string, TMesh*> m_MeshesMap;
	std::unordered_map<std::string, TPointLight*> m_PointLightsMap;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pPointLightConstantBuffer;

	TDXRenderDevice* m_TDXRenderDevice;

};

#endif
