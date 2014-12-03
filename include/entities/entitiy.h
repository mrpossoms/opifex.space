#ifndef SPACE_ENTITY
#define SPACE_ENTITY

#include "./Engine.h"

struct Entity{
	OPvec3  Position;
	OPfloat Scale;
};

struct ReferenceFrame{
	OPvec3 up;
	OPvec3 forward;
	OPvec3 left;
};

#endif