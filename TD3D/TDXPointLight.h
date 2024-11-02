#ifndef TDXPOINTLIGHT_H
#define TDXPOINTLIGHT_H

#include "TPointLight.h"
#include "TDXObjectManager.h"
#include "T.h"

class TDXPointLight : TPointLight
{
friend class TDXObjectManager;

public:
	TDXPointLight(TVector3 position, TVector3 color, float range, float intencity);

	virtual void SetPosition(float x, float y, float z) override;
	virtual void SetColor(float r, float g, float b) override;
	virtual void SetRange(float range) override;
	virtual void SetIntencity(float intencity) override;

private:
	TVector3 m_Position;
	TVector3 m_Color;
	float m_Range;
	float m_Intencity;
};

#endif
