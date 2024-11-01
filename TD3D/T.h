#ifndef T_H
#define T_H

struct TVertexColor
{
	struct
	{
		float x;
		float y;
		float z;
	} pos;
	struct
	{
		float r;
		float g;
		float b;
		float a;
	} color;
};

struct TPosition
{
	float x;
	float y;
	float z;
};

struct TVector4
{
	float x;
	float y;
	float z;
	float w;
};

#endif
