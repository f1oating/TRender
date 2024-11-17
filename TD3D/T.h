#ifndef T_H
#define T_H

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

struct TVector2
{
	float x;
	float y;
};

struct TVertexPT
{
	TVector3 pos;
	TVector2 tex;
};

struct TVertexSkybox
{
	TVector3 pos;
	TVector3 tex;
};

#endif
