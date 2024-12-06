#ifndef TDXBUFFERMANAGER_H
#define TDXBUFFERMANAGER_H

#include <string>
#include <unordered_map>
#include <d3d11.h>

class TDXBufferManager
{
public:
	~TDXBufferManager();

	void CreateStaticVertexBuffer(std::string name, void* vertices, unsigned int numVertices, unsigned int vertexSize, ID3D11Device* device);
	void CreateStaticIndexBuffer(std::string name, unsigned int* indices, unsigned int numIndices, ID3D11Device* device);
	void CreateDynamicConstantBuffer(std::string name, unsigned int structSize, ID3D11Device* device);
	void CreateDynamicVertexBuffer(std::string name, void* vertices, unsigned int numVertices, unsigned int vertexSize, ID3D11Device* device);
	void CreateDynamicIndexBuffer(std::string name, unsigned int* indices, unsigned int numIndices, ID3D11Device* device);
	void CreateDynamicStructuredBuffer(std::string name, unsigned int structSize, unsigned int structCount, ID3D11Device* device);

	void UpdateStaticVertexBuffer(std::string name, void* vertices, ID3D11DeviceContext* context);
	void UpdateStaticIndexBuffer(std::string name, unsigned int* indices, ID3D11DeviceContext* context);
	void UpdateDynamicConstantBuffer(std::string name, void* constantBufferStruct, unsigned int structSize, ID3D11DeviceContext* context);
	void UpdateDynamicVertexBuffer(std::string name, void* vertices, unsigned int numVertices, unsigned int vertexSize, ID3D11DeviceContext* context);
	void UpdateDynamicIndexBuffer(std::string name, unsigned int* indices, unsigned int numIndices, ID3D11DeviceContext* context);
	void UpdateDynamicStructuredBuffer(std::string name, void* structuredBufferStruct, unsigned int structSize, unsigned int structCount, ID3D11DeviceContext* context);

	void BindVertexBuffer(std::string vertexName, UINT stride, UINT offset, ID3D11DeviceContext* context);
	void BindIndexBuffer(std::string indexName, ID3D11DeviceContext* context);
	void VBindConstantBuffer(std::string constantName, unsigned int slot, ID3D11DeviceContext* context);
	void PBindConstantBuffer(std::string constantName, unsigned int slot, ID3D11DeviceContext* context);
	void GBindConstantBuffer(std::string constantName, unsigned int slot, ID3D11DeviceContext* context);

	ID3D11Buffer* GetBuffer(std::string name);

	void DeleteBuffer(std::string name);

private:
	std::unordered_map<std::string, ID3D11Buffer*> m_BuffersMap;

};

#endif
