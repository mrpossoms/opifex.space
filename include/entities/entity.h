#ifndef SPACE_ENTITY
#define SPACE_ENTITY

#include "./Engine.h"

//-----------------------------------------------------------------------------
//    ___ _               _      
//   / __| |_ _ _ _  _ __| |_ ___
//   \__ \  _| '_| || / _|  _(_-<
//   |___/\__|_|  \_,_\__|\__/__/
//          

// This struct is used as a convienient constantly refreshed
// definition of an object's local coordinate system's orientation.
// This is especially useful as we rotate.
struct ReferenceFrame{
	OPvec3 up;  
	OPvec3 forward;
	OPvec3 left;
};

#endif