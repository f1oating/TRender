#ifndef TDXSHADERMANAGER_H
#define TDXSHADERMANAGER_H

#include "TDXRenderDevice.h"

class TDXShaderManager
{
public:
	TDXShaderManager(TDXRenderDevice* tdxRenderDevice);
	~TDXShaderManager();

	void AddShaders(std::string name, std::string path, D3D11_INPUT_ELEMENT_DESC* ied, UINT size);
	void BindShaders(std::string name);

	ID3D11VertexShader* GetVertexShader(std::string name);
	ID3D11PixelShader* GetPixelShader(std::string name);

private:
	std::unordered_map<std::string, ID3D11VertexShader*> m_VertexShadersMap;
	std::unordered_map<std::string, ID3D11PixelShader*> m_PixelShadersMap;
	std::unordered_map<std::string, ID3D11InputLayout*> m_InputLayoutsMap;

	Microsoft::WRL::ComPtr <ID3DBlob> m_pBlob;

	TDXRenderDevice* m_TDXRenderDevice;

};

#endif
