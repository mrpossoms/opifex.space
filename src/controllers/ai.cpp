#include "./include/controllers/ai.h"

struct Ship* nearestShip(struct Ship* me, OPentHeap* enemies)
{
	struct Ship* allEnemies = (struct Ship*)enemies->Entities;
	struct Ship* closest = allEnemies;
	OPvec3 dif = closest->position - me->position;
	OPfloat dist = OPvec3dot(&dif, &dif);

	for(register OPint i = enemies->MaxIndex; i--;){
		OPentHeapIsLiving(enemies, i);

		dif = allEnemies[i].position - me->position;
		OPfloat newDist = OPvec3dot(&dif, &dif);

		if(newDist < dist){
			dist = newDist;
			closest = allEnemies + i;
		}
	}

	// if no living enemies exist, then return nothing!
	if(!enemies->MaxIndex){
		return NULL;
	}

	return closest;
}

void aiHandler(struct Ship* ship, OPfloat elapsedTime)
{
	OPentHeap* targets;

	if((ship->properties & SHIP_TEAM_MASK) == SHIP_TEAM_RED){
		targets = SHIPS_BLUE;
	}
	else{
		targets = SHIPS_RED;
	}

	// go after the closest!
	struct Ship* target = nearestShip(ship, targets);

	// movement vars
	OPquat netRotation = OPquatIdentity;
	OPquat rotation;
	OPfloat angle = 1.5f * elapsedTime;

	if(0 && target){
		// attack the ship!
	}
	else{
		// do stuff
		OPint action = OPrandom() * 3;

		switch(action){
			case 0:
				rotation = OPquatCreateRot((OPvec3*)&ship->frame.up, angle);
				netRotation = OPquatMul(&netRotation, &rotation);
				break;
			case 1:
				rotation = OPquatCreateRot((OPvec3*)&ship->frame.left, angle);
				netRotation = OPquatMul(&netRotation, &rotation);
				break;
			case 2:
				rotation = OPquatCreateRot((OPvec3*)&ship->frame.forward, angle);
				netRotation = OPquatMul(&netRotation, &rotation);
				break;
		}
	}

	ship->attitude = OPquatMul(&ship->attitude, &netRotation);

	// accelerate!
	ShipThrust(ship, OPvec2Zero, ship->engine = 2.5f, elapsedTime);
}

struct Controller InitControllerAI(struct Ship* ship)
{
	struct Controller controller = {
		ship,
		aiHandler
	};

	return controller;
}