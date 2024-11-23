#ifndef T_H
#define T_H

#include <string>

constexpr const char* SKYBOX_SHADER = "SKYBOX";
constexpr const char* SPRITE_SHADER_TEXTURE = "SPRITE_TEXTURE";
constexpr const char* SPRITE_SHADER_COLOR = "SPRITE_COLOR";
constexpr const char* MESH_SHADER = "MESH";

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

#endif
