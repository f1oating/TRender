#ifndef T_H
#define T_H

#include <string>

constexpr const char* SKYBOX_SHADER = "SKYBOX";
constexpr const char* SPRITE_SHADER_TEXTURE = "SPRITE_TEXTURE";
constexpr const char* SPRITE_SHADER_COLOR = "SPRITE_COLOR";
constexpr const char* GEOMETRY_SHADER = "GEOMETRY_SHADER";
constexpr const char* LIGHT_SHADER = "LIGHT";

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

struct TVertexScreenQuad
{
	TVector3 pos;
	TVector2 tex;
};

struct TVertexSpriteTexture
{
	TVector2 pos;
	TVector2 tex;
};

struct TVertexSpriteColor
{
	TVector2 pos;
	TVector4 color;
};

struct TVertexMesh
{
	TVector3 pos;
	TVector2 tex;
	TVector3 normal;
	TVector3 tangent;
};

struct TVertexSkybox
{
	TVector3 pos;
	TVector3 tex;
};

struct Light
{
	int Type;
	TVector3 Position;
	TVector3 Direction;
	TVector3 Color;
	float Intensity;
	float Range;
	float SpotAngle;
};

#endif
