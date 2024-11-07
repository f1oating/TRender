#ifndef TDXSHADERMANAGER_H
#define TDXSHADERMANAGER_H

#include <unordered_map>
#include "d3d11.h"
#include <wrl/client.h>

class TDXRenderDevice;

class TDXShaderManager
{
public:
	TDXShaderManager();
	~TDXShaderManager();

	void AddVertexShader(std::string name, LPCWSTR path, D3D11_INPUT_ELEMENT_DESC* ied, UINT size, ID3D11Device* device);
	void AddPixelShader(std::string name, LPCWSTR pathPixel, ID3D11Device* device);

	void BindVertexShader(std::string name, ID3D11DeviceContext* deviceContext);
	void BindPixelShader(std::string name, ID3D11DeviceContext* deviceContext);

	ID3D11VertexShader* GetVertexShader(std::string name);
	ID3D11PixelShader* GetPixelShader(std::string name);

private:
	std::unordered_map<std::string, ID3D11VertexShader*> m_VertexShadersMap;
	std::unordered_map<std::string, ID3D11PixelShader*> m_PixelShadersMap;
	std::unordered_map<std::string, ID3D11InputLayout*> m_InputLayoutsMap;

	Microsoft::WRL::ComPtr <ID3DBlob> m_pBlob;

};

#endif
