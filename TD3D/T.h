#ifndef T_H
#define T_H

#include "T3DMath.h"

struct TVertex {
	struct
	{
		float x;
		float y;
	} pos;
	struct
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	} color;
};

struct TCOLOR {
    float r;
    float g;
    float b;
    float a;
};

#define T_OK    0
#define T_FAIL  0x82000001

#endif
