#ifndef TDXFEATURECONTROLLER_H
#define TDXFEATURECONTROLLER_H

#include <d3d11.h>

class TDXFeatureController
{
public:
	void ChangeDepthStencilComparison(bool flag, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void ChangeRasterizerCulling(bool flag, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void ChangeBlendState(bool flag, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
};

#endif
