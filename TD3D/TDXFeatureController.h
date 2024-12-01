#ifndef TDXFEATURECONTROLLER_H
#define TDXFEATURECONTROLLER_H

#include <d3d11.h>
#include <wrl/client.h>

class TDXFeatureController
{
public:
	void Initizialize(ID3D11Device* device);

	void ChangeDepthStencilComparison(bool flag, ID3D11DeviceContext* deviceContext);
	void ChangeRasterizerCulling(bool flag, ID3D11DeviceContext* deviceContext);
	void ChangeBlendState(bool flag, ID3D11DeviceContext* deviceContext);

private:
	void CreateDepthStencilComparisons(ID3D11Device* device);
	void CreateRasterizerCullings(ID3D11Device* device);
	void CreateBlendStates(ID3D11Device* device);

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthEnable;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthDisable;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pRasterizerStateEnable;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pRasterizerStateDisable;

	Microsoft::WRL::ComPtr<ID3D11BlendState> m_pBlendStateEnable;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_pBlendStateDisable;
};

#endif
