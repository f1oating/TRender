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
	struct
	{
		float x;
		float y;
		float z;
	} normal;
};

struct TVector4
{
	float x;
	float y;
	float z;
	float w;
};

struct TVector3
{
	float x;
	float y;
	float z;
};

#endif
