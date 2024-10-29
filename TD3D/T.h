#ifndef T_H
#define T_H

#include "T3DMath.h"

struct TVertex {
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

#define T_OK    0
#define T_FAIL  0x82000001

#endif
