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

	void CreateSampler(ID3D11Device* device, ID3D11DeviceContext* context);

	void AddTexture(std::string name, std::string path, ID3D11Device* device);
	void AddCubeMapTexture(std::string name, std::string path, std::string ext, ID3D11Device* device);

	void BindTexture(std::string name, ID3D11DeviceContext* context);

	void DeleteTexture(std::string name);

private:
	unsigned char* LoadCubeMapTextureData(const std::string& filename, int& width, int& height, int& channels);

	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_TexturesMap;

};

#endif TDXTEXTUREMANAGER_H
