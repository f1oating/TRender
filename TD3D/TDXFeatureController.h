#ifndef TDXFEATURECONTROLLER_H
#define TDXFEATURECONTROLLER_H

#include <d3d11.h>

class TDXFeatureController
{
public:
	void ChangeDepthStencilComparison(D3D11_COMPARISON_FUNC comparisonFunc, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void ChangeRasterizerCulling(D3D11_CULL_MODE cullMode, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
};

#endif
