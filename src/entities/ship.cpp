#include "./include/entities/ship.h"

const OPfloat SHIP_SPAWN_RADIUS = 20.0f;

//-----------------------------------------------------------------------------
//     ___ _     _          _    
//    / __| |___| |__  __ _| |___
//   | (_ | / _ \ '_ \/ _` | (_-<
//    \___|_\___/_.__/\__,_|_/__/
//                                                   
OPmesh*    SHIP_MESH;
OPtexture *SHIP_COLORMAP_RED, *SHIP_COLORMAP_BLUE;
OPtexture *SHIP_COLORMAP_ENGINE;

OPeffect   SHIP_EFFECT;

OPentHeap* SHIPS_RED;
OPentHeap* SHIPS_BLUE;

//-----------------------------------------------------------------------------
//    ___      _ _   _      _ _         _   _          
//   |_ _|_ _ (_) |_(_)__ _| (_)_____ _| |_(_)___ _ _  
//    | || ' \| |  _| / _` | | |_ / _` |  _| / _ \ ' \ 
//   |___|_||_|_|\__|_\__,_|_|_/__\__,_|\__|_\___/_||_|
//                                                     
OPint ShipInit()
{
	OPcmanLoad("fighter.opm");
	OPcmanLoad("fighterBlue.png");
	OPcmanLoad("fighterRed.png");
	OPcmanLoad("fighterEngineMask.png");

	// load and compile shaders for the ship effect
	SHIP_EFFECT = OPrenderGenEffect(
		"Common/PositionTextureNormal.vert",
		"Common/TexturedDirectionalLight.frag",
		OPATTR_POSITION | OPATTR_UV | OPATTR_NORMAL,
		"Ship Effect",
		0
	);

	OPuint teamHeapBytes = OPentHeapBytes(
		sizeof(struct Ship), // size of a single ship
		SHIP_MAX_TEAM_SIZE  // number of ships in a team
	);

	// allocate two ent heaps (one for each team) in a
	// contiguous memory block
	void* shipSegment = OPalloc(teamHeapBytes * 2);

	// now select the red entHeap as the first half of the
	// memory segment...
	SHIPS_RED  = OPentHeapCreate(
		shipSegment,
		sizeof(struct Ship),
		SHIP_MAX_TEAM_SIZE
	);

	// and the blue entHeap as the second half
	SHIPS_BLUE = OPentHeapCreate(
		(ui8*)shipSegment + teamHeapBytes,
		sizeof(struct Ship),
		SHIP_MAX_TEAM_SIZE
	);

	// snag the mesh and texture for easy of use later
	SHIP_MESH  = (OPmesh*)OPcmanGet("fighter.opm");
	SHIP_COLORMAP_BLUE = (OPtexture*)OPcmanGet("fighterBlue.png");
	SHIP_COLORMAP_RED  = (OPtexture*)OPcmanGet("fighterRed.png");
	SHIP_COLORMAP_ENGINE = (OPtexture*)OPcmanGet("fighterEngineMask.png");

	return 1;
}

OPint ShipDestroy()
{
	OPcmanUnload("fighter.opm");
	OPcmanUnload("fighter.png");
}

struct Ship* ShipSpawn(OPuint properties)
{
	struct Ship* ship = NULL;
	OPint activatedIndex;

	OPuint team   = properties & SHIP_TEAM_MASK;
	OPfloat theta; 

	switch(team){
		case SHIP_TEAM_RED:
			OPentHeapActivate(SHIPS_RED, &activatedIndex);

			if(activatedIndex < 0) return NULL;

			ship = ((struct Ship*)SHIPS_RED->Entities) + activatedIndex;
			theta = 0;

			break;
		case SHIP_TEAM_BLUE:
			OPentHeapActivate(SHIPS_BLUE, &activatedIndex);

			if(activatedIndex < 0) return NULL;

			ship = ((struct Ship*)SHIPS_BLUE->Entities) + activatedIndex;
			theta = OPpi;
			break;
	}

	OPbzero(ship, sizeof(struct Ship)); // set all values to zero

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

	// reset the rotation
	ship->attitude = OPquatCreateLookAt(
		&ship->position,
		(OPvec3*)&OPvec3Zero
	);
	
	ship->angularVelocity = OPquatIdentity; // reset the angular velocity too
	ship->properties = properties;     // assign the properties
	ship->hp = 3.0f;                   // reset the health


	// TODO calculate the orientation to point at the enemy team

	ShipUpdateReferenceFrame(ship);

	return ship;
}

//-----------------------------------------------------------------------------
//    _   _          _      _         ___             _   _             
//   | | | |_ __  __| |__ _| |_ ___  | __|  _ _ _  __| |_(_)___ _ _  ___
//   | |_| | '_ \/ _` / _` |  _/ -_) | _| || | ' \/ _|  _| / _ \ ' \(_-<
//    \___/| .__/\__,_\__,_|\__\___| |_| \_,_|_||_\__|\__|_\___/_||_/__/
//         |_|                                                          
void shipsUpdate(OPentHeap* ships, OPfloat elapsedTime)
{
	// iterate over all the ships in the array. Tell the compiler
	// to keep index i in a register as a perf hint
	for(register OPint i = ships->MaxIndex; i--;){
		OPentHeapIsLiving(ships, i); // this will skip ships that are not active (dead)

		struct Ship* ship = (struct Ship*)ships->Entities + i; // proxy pointer for convenience
		OPquat omega = ship->angularVelocity;                  
		OPquat decay;

		// skip ships that are dead
		if(ship->hp <= 0){
			OPentHeapKill(ships, i); // disable the ship in the ent heap
			continue;
		}

		// ShipThrust(ship, OPvec2Zero, 1, elapsedTime);

		// update the position vector in respect to time
		// PS (this is why operator overloads are cool)
		ship->position += ship->velocity * elapsedTime;

		// update the attitude 
		OPquatScl(&omega, elapsedTime); // angular velocity with respect to time
		ship->attitude = OPquatMul(&ship->attitude, &omega);

		// It might not be realistic, but apply friction to the ship :)
		ship->velocity -= ship->velocity * elapsedTime * 0.6f;

		omega = OPquatScl(&omega, 0.99);
		OPquat iTerm = OPquatScl((OPquat*)&OPquatIdentity, 0.01);
		ship->angularVelocity = OPquatAdd(&iTerm, &omega);

		ShipUpdateReferenceFrame(ship);

		// Cool down the guns!
		if(ship->gunCoolDown > 0){
			ship->gunCoolDown -= elapsedTime;
		}
		else{
			ship->gunCoolDown = 0;
		}
	}
}

void ShipBatchUpdate(OPfloat elapsedTime)
{
	shipsUpdate(SHIPS_RED, elapsedTime);
	shipsUpdate(SHIPS_BLUE, elapsedTime);
}

void ShipFire(struct Ship* ship)
{
	if(ship->gunCoolDown > 0) return;

	struct Projectile p = {
		ship->position,                            // starting point of the projectile
		ship->velocity + ship->frame.forward * -10, // velocity of the projectile
		ship->properties & SHIP_TEAM_MASK,         // use the ship's team
		5 // live for 5 seconds
	};

	ship->gunCoolDown = SHIP_GUN_COOLDOWN;
	ProjectileSpawn(p);
}


//-----------------------------------------------------------------------------
//    ___                  _             ___             _   _             
//   |   \ _ _ __ ___ __ _(_)_ _  __ _  | __|  _ _ _  __| |_(_)___ _ _  ___
//   | |) | '_/ _` \ V  V / | ' \/ _` | | _| || | ' \/ _|  _| / _ \ ' \(_-<
//   |___/|_| \__,_|\_/\_/|_|_||_\__, | |_| \_,_|_||_\__|\__|_\___/_||_/__/
//                               |___/                               
void shipDrawTeam(OPentHeap* ships, OPint count, OPcam* camera)
{
	struct Ship* shipArray = (struct Ship*)ships->Entities;
	OPuint team = shipArray->properties & SHIP_TEAM_MASK;

	// Send the new camera view and projection, and effects to the 
	OPtextureClearActive();
	OPbindMeshEffectWorldCam(SHIP_MESH, &SHIP_EFFECT, (OPmat4*)&OPmat4Identity, camera);
	OPtextureBind(team == SHIP_TEAM_RED ? SHIP_COLORMAP_RED : SHIP_COLORMAP_BLUE);
	OPrenderParami("uTexture", 0);
	OPtextureBind(SHIP_COLORMAP_ENGINE);
	OPrenderParami("uEngineMask", 1);

	for(register OPint i = ships->MaxIndex; i--;){
		OPentHeapIsLiving(ships, i); // this will skip ships that are not active (dead)

		struct Ship* ship = shipArray + i; // proxy pointer for convenience
		OPmat4 world = OPmat4Identity;

		// skip ships that are dead
		if(ship->hp <= 0) continue;

		// create the new world matrix for this ship, then
		// send it on over to the video card
		OPmat4buildQuat(&world, &ship->attitude);
		OPmat4translate(&world, &ship->position);
		OPrenderParamMat4("uWorld", &world);
		OPrenderParamf("uEnginePower", ship->engine);

		// finally, render the ship
		OPrenderMesh();
	}
}

void ShipBatchDraw(OPcam* camera)
{
	shipDrawTeam(SHIPS_RED, SHIP_MAX_TEAM_SIZE, camera);
	shipDrawTeam(SHIPS_BLUE, SHIP_MAX_TEAM_SIZE, camera);
}
