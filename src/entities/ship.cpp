#include "./include/entities/ship.h"

const OPfloat SHIP_SPAWN_RADIUS = 20.0f;

OPint ShipSpawn(struct Ship* ship, OPuint properties)
{
	OPuint team   = properties & SHIP_TEAM_MASK;
	OPfloat theta; 

	OPbzero(ship, sizeof(struct Ship)); // set all values to zero

	ship->attitude        = OPquatIdentity; // reset the rotation
	ship->angularVelocity = OPquatIdentity; // reset the angular velocity too
	ship->properties = properties;     // assign the properties
	ship->hp = 3.0f;                   // reset the health

	switch(team){
		case SHIP_TEAM_RED:
			theta = 0;
			break;
		case SHIP_TEAM_BLUE:
			theta = OPpi;
			break;
	}

	// caculate a new spawn position given the angle determined
	// by the switch statement above
	OPvec3 spawnPosition = {
		OPcos(theta + (OPrandom() - 0.5f) * OPpi / 2),
		OPsin((OPrandom() - 0.5f) * OPpi),
		OPsin(theta + (OPrandom() - 0.5f) * OPpi / 2)
	};

	// update the position
	OPvec3norm(&ship->position, &spawnPosition);
	ship->position *= SHIP_SPAWN_RADIUS;

	// TODO calculate the orientation to point at the enemy team

	ShipUpdateReferenceFrame(ship);
}

//-----------------------------------------------------------------------------
//    _   _          _      _         ___             _   _             
//   | | | |_ __  __| |__ _| |_ ___  | __|  _ _ _  __| |_(_)___ _ _  ___
//   | |_| | '_ \/ _` / _` |  _/ -_) | _| || | ' \/ _|  _| / _ \ ' \(_-<
//    \___/| .__/\__,_\__,_|\__\___| |_| \_,_|_||_\__|\__|_\___/_||_/__/
//         |_|                                                          
void ShipUpdateMovement(struct Ship* ships, OPint count, OPfloat elapsedTime)
{
	// iterate over all the ships in the array. Tell the compiler
	// to keep index i in a register as a perf hint
	for(register OPint i = count; i--;){
		struct Ship* ship = ships + i; // proxy pointer for convenience
		OPquat omega;                  

		// update the position vector in respect to time
		// PS (this is why operator overloads are cool)
		ship->position += ship->velocity * elapsedTime;

		// update the attitude 
		OPquatScl(&omega, elapsedTime); // angular velocity with respect to time
		ship->attitude = OPquatMul(&ship->attitude, &omega);

		// It might not be realistic, but apply friction to the ship :)
		ship->velocity += ship->velocity * elapsedTime * 0.1f;

		ShipUpdateReferenceFrame(ship);
	}
}

void ShipBatchUpdate(struct Ship* ships, OPint count, OPfloat elapsedTime)
{
	ShipUpdateMovement(ships, count, elapsedTime);
}

//-----------------------------------------------------------------------------
//    ___                  _             ___             _   _             
//   |   \ _ _ __ ___ __ _(_)_ _  __ _  | __|  _ _ _  __| |_(_)___ _ _  ___
//   | |) | '_/ _` \ V  V / | ' \/ _` | | _| || | ' \/ _|  _| / _ \ ' \(_-<
//   |___/|_| \__,_|\_/\_/|_|_||_\__, | |_| \_,_|_||_\__|\__|_\___/_||_/__/
//                               |___/                                     
void ShipBatchDraw(struct Ship* ships, OPint count)
{

}
