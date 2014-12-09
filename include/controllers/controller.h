#ifndef SMASH_CONTROLLER
#define SMASH_CONTROLLER

#include "./Engine.h"
#include "./include/states/game.h"
#include "./include/entities/ship.h"
#include "./include/entities/projectile.h"

//-----------------------------------------------------------------------------
//    ___ _               _      
//   / __| |_ _ _ _  _ __| |_ ___
//   \__ \  _| '_| || / _|  _(_-<
//   |___/\__|_|  \_,_\__|\__/__/
//          
struct Controller{
	struct Ship* ship;                                       // pointer to the ship that this controller interacts with
	void (*handler)(struct Ship* ship, OPfloat elapsedTime); // callback function that interacts with the ship
};

#endif