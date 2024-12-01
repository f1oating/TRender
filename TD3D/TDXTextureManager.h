#ifndef TDXTEXTUREMANAGER_H
#define TDXTEXTUREMANAGER_H

#include <string>
#include <d3d11.h>
#include <unordered_map>

class TDXTextureManager
{
public:
	TDXTextureManager();
	~TDXTextureManager();

	void CreateSampler(std::string, ID3D11Device* device, ID3D11DeviceContext* context);

	void AddTexture(std::string name, std::string path, ID3D11Device* device);
	void AddTexture(std::string name, unsigned int r, unsigned int g, unsigned int b, unsigned int a, ID3D11Device* device);
	void AddCubeMapTexture(std::string name, std::string path, std::string ext, ID3D11Device* device);

	void BindTexture(std::string name, ID3D11DeviceContext* context);
	void BindSampler(std::string name, ID3D11DeviceContext* context);

	void DeleteTexture(std::string name);

private:
	unsigned char* LoadCubeMapTextureData(const std::string& filename, int& width, int& height, int& channels);

	std::unordered_map<std::string, ID3D11SamplerState*> m_SamplersMap;
	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_TexturesMap;

};

#endif TDXTEXTUREMANAGER_H
