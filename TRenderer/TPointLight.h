#ifndef TPOINTLIGHT_H
#define TPOINTLIGHT_H

class TPointLight
{
public:
	virtual ~TPointLight() {};

	virtual void SetPosition(float x, float y, float z) = 0;
	virtual void SetColor(float r, float g, float b) = 0;
	virtual void SetRange(float range) = 0;
	virtual void SetIntencity(float intencity) = 0;
};

#endif
