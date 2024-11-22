#ifndef TDXBUFFERMANAGER_H
#define TDXBUFFERMANAGER_H

#include <string>
#include <unordered_map>
#include <d3d11.h>

class TDXBufferManager
{
public:
	~TDXBufferManager();

	void CreateStaticVertexBuffer(std::string name, void* vertices, unsigned short numVertices, unsigned short vertexSize, ID3D11Device* device);

	void CreateStaticIndexBuffer(std::string name, unsigned short* indices, unsigned short numIndices, ID3D11Device* device);

	void CreateDynamicConstantBuffer(std::string name, unsigned short structSize, ID3D11Device* device);

	void UpdateStaticVertexBuffer(std::string name, void* vertices, ID3D11DeviceContext* context);

	void UpdateStaticIndexBuffer(std::string name, unsigned short* indices, ID3D11DeviceContext* context);

	void UpdateDynamicConstantBuffer(std::string name, void* constantBufferStruct, unsigned short structSize, ID3D11DeviceContext* context);

	void BindVertexBuffer(std::string vertexName, UINT stride, UINT offset, ID3D11DeviceContext* context);
	void BindIndexBuffer(std::string indexName, ID3D11DeviceContext* context);
	void BindConstantBuffer(std::string constantName, unsigned short slot, ID3D11DeviceContext* context);

	void DeleteBuffer(std::string name);

private:
	std::unordered_map<std::string, ID3D11Buffer*> m_BuffersMap;

};

#endif
