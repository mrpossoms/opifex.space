#include "./include/controllers/ai.h"

struct Ship* nearestShip(struct Ship* me, OPentHeap* enemies)
{
	struct Ship* allEnemies = (struct Ship*)enemies->Entities;
	struct Ship* closest = allEnemies;
	OPfloat dist = OPvec3distSquare(&closest->position, &me->position);

	for(register OPint i = enemies->MaxIndex; i--;){
		OPentHeapIsLiving(enemies, i);

		OPfloat newDist = OPvec3distSquare(&allEnemies[i].position, &me->position);

		if(newDist < dist && closest->hp > 0){
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

	if(target){
		// attack the ship!
		OPquat targetOrientation = OPquatCreateLookAt(&ship->position, &target->position);
		ship->attitude = OPquatLerp(&targetOrientation, &ship->attitude, elapsedTime * 2 * OPrandom());

		// fire if close enough, but also randomize
		if(OPvec3distSquare(&target->position, &ship->position) < 100 * OPrandom() && OPrandom() < 0.5f){
			ShipFire(ship);
		}

		// accelerate!
		ShipThrust(ship, OPvec2Zero, ship->engine = 2.5f, elapsedTime);
	}
	else{
		// Select a rotation manuver at random.
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
	
		// apply the rotaion
		ship->attitude = OPquatMul(&ship->attitude, &netRotation);

		// accelerate!
		ShipThrust(ship, OPvec2Zero, ship->engine = 2.5f, elapsedTime);
	}
}

struct Controller InitControllerAI(struct Ship* ship)
{
	struct Controller controller = {
		ship,
		aiHandler
	};

	return controller;
}