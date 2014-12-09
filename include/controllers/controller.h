#ifndef SMASH_CONTROLLER
#define SMASH_CONTROLLER

#include "./Engine.h"
#include "./include/states/game.h"
#include "./include/entities/ship.h"
#include "./include/entities/projectile.h"

struct Controller{
	struct Ship* ship;
	void (*handler)(struct Ship* ship, OPfloat elapsedTime);
};

#endif