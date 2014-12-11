#ifndef SPACE_SHIP
#define SPACE_SHIP

#include "./Engine.h"
#include "./include/states/game.h"
#include "./include/entities/entity.h"
#include "./include/entities/projectile.h"

//-----------------------------------------------------------------------------
//     ___             _            _      
//    / __|___ _ _  __| |_ __ _ _ _| |_ ___
//   | (__/ _ \ ' \(_-<  _/ _` | ' \  _(_-<
//    \___\___/_||_/__/\__\__,_|_||_\__/__/
//                                         
#define SHIP_TEAM_RED         0b00
#define SHIP_TEAM_BLUE        0b01

#define SHIP_TEAM_MASK       0b01



#define SHIP_GUN_COOLDOWN 0.125f

//-----------------------------------------------------------------------------
//    ___ _               _      
//   / __| |_ _ _ _  _ __| |_ ___
//   \__ \  _| '_| || / _|  _(_-<
//   |___/\__|_|  \_,_\__|\__/__/
//                               
struct Ship{
	OPvec3 position;             // the location of the ship in space (12)
	OPvec3 velocity;             // the direction, and speed of the ship's motion (12)
	OPquat attitude;             // representation of the ship's rotation in space (16)
	struct ReferenceFrame frame; // defines the local coordinate system for the ship (36)
	OPfloat hp;                   // health (4)
	OPuint properties;           // bit field for storing simple data about the ship (4)
	OPfloat engine;      // Aesthetic variable to show the engine firing up (4)
	OPfloat gunCoolDown; // Timer between gun shots (4)
};
// 92 bytes per ship

#define SHIP_MAX_TEAM_SIZE ((12 * 1024) / sizeof(struct Ship))

//-----------------------------------------------------------------------------
//     ___ _     _          _    
//    / __| |___| |__  __ _| |___
//   | (_ | / _ \ '_ \/ _` | (_-<
//    \___|_\___/_.__/\__,_|_/__/
//                               
extern OPentHeap *SHIPS_BLUE, *SHIPS_RED;

//-----------------------------------------------------------------------------
//    ___         _       _                     
//   | _ \_ _ ___| |_ ___| |_ _  _ _ __  ___ ___
//   |  _/ '_/ _ \  _/ _ \  _| || | '_ \/ -_|_-<
//   |_| |_| \___/\__\___/\__|\_, | .__/\___/__/
//                            |__/|_|           
OPint ShipInit();
OPint ShipDestroy();

struct Ship* ShipSpawn(OPuint properties);
void ShipFire(struct Ship* ship);

void ShipBatchUpdate(OPfloat elapsedTime);
void ShipBatchDraw(OPcam* camera);

//-----------------------------------------------------------------------------
//    ___      _ _             _    __              _   _             
//   |_ _|_ _ | (_)_ _  ___ __| |  / _|_  _ _ _  __| |_(_)___ _ _  ___
//    | || ' \| | | ' \/ -_) _` | |  _| || | ' \/ _|  _| / _ \ ' \(_-<
//   |___|_||_|_|_|_||_\___\__,_| |_|  \_,_|_||_\__|\__|_\___/_||_/__/
//                                                                    
inline void ShipUpdateReferenceFrame(struct Ship* ship)
{
	// update the reference frame with the new attitude
	ship->frame.up      = OPquatRot(&ship->attitude, (OPvec3*)&OPvec3Up);
	ship->frame.forward = OPquatRot(&ship->attitude, (OPvec3*)&OPvec3Forward);
	ship->frame.left    = OPquatRot(&ship->attitude, (OPvec3*)&OPvec3Left);
}

inline void ShipThrust(struct Ship* ship, OPvec2 strafe, OPfloat engine, OPfloat elapsedTime)
{
	// get a proxy var for the reference frame
	struct ReferenceFrame* frame = &ship->frame;

	// Respect that time yo (for accelerations too)
	strafe *= elapsedTime;
	engine *= elapsedTime;

	// apply the net acceleration to the ship
	ship->velocity += frame->up * strafe.y + 
	                  frame->left * strafe.x +
	                  frame->forward * -engine;
}

#endif