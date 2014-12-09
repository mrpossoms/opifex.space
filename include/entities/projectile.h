#ifndef SPACE_PROJECTILE
#define SPACE_PROJECTILE

#include "./Engine.h"
#include "./include/entities/ship.h"

//-----------------------------------------------------------------------------
//    ___ _               _      
//   / __| |_ _ _ _  _ __| |_ ___
//   \__ \  _| '_| || / _|  _(_-<
//   |___/\__|_|  \_,_\__|\__/__/
//          
struct Projectile{
	OPvec3    position;
	OPvec3    velocity;
	OPint   team;
	OPfloat life;
};

OPint ProjectileInit();
OPint ProjectileDestroy();

struct Projectile* ProjectileSpawn(struct Projectile proj);

void ProjectileBatchUpdate(OPfloat elapsedTime);
void ProjectileBatchDraw(OPcam* camera);

#endif