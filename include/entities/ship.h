#ifndef SPACE_SHIP
#define SPACE_SHIP

#include "./Engine.h"
#include "./include/entities/entity.h"

//-----------------------------------------------------------------------------
//     ___             _            _      
//    / __|___ _ _  __| |_ __ _ _ _| |_ ___
//   | (__/ _ \ ' \(_-<  _/ _` | ' \  _(_-<
//    \___\___/_||_/__/\__\__,_|_||_\__/__/
//                                         
#define SHIP_TEAM_RED         0b00
#define SHIP_TEAM_BLUE        0b01
#define SHIP_CONTROLLER_AI    0b00
#define SHIP_CONTROLLER_HUMAN 0b10

#define SHIP_TEAM_MASK       0b01
#define SHIP_CONTROLLER_MASK 0b10

//-----------------------------------------------------------------------------
//    ___ _               _      
//   / __| |_ _ _ _  _ __| |_ ___
//   \__ \  _| '_| || / _|  _(_-<
//   |___/\__|_|  \_,_\__|\__/__/
//                               
struct Ship{
	OPvec3 position;             // the location of the ship in space
	OPvec3 velocity;             // the direction, and speed of the ship's motion
	OPquat attitude;             // representation of the ship's rotation in space
	OPquat angularVelocity;      // speed and angle at which the ship is spinning
	struct ReferenceFrame frame; // defines the local coordinate system for the ship
	OPint  hp;                   // health
	OPuint properties;           // bit field for storing simple data about the ship
};

//-----------------------------------------------------------------------------
//    ___         _       _                     
//   | _ \_ _ ___| |_ ___| |_ _  _ _ __  ___ ___
//   |  _/ '_/ _ \  _/ _ \  _| || | '_ \/ -_|_-<
//   |_| |_| \___/\__\___/\__|\_, | .__/\___/__/
//                            |__/|_|           
OPint ShipSpawn(struct Ship* ship, OPuint properties);

void ShipBatchUpdate(struct Ship* ships, OPint count, OPfloat elapsedTime);
void ShipBatchDraw(struct Ship* ships, OPint count);

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
	                  frame->forward * engine;
}

#endif