#include "./include/controllers/player.h"

OPvec3 PLAYER_CAMERA_TARGET;

void playerKeyboardControls(struct Ship* ship, OPfloat elapsedTime, OPquat* netRotation)
{
	OPfloat angle = 1.5f * elapsedTime; // define angle in respect to time

	if(OPkeyboardIsDown(OPKEY_A)){
		OPquat rotation = OPquatCreateRot((OPvec3*)&ship->frame.up, -angle);
		*netRotation = OPquatMul(netRotation, &rotation);
	}
	if(OPkeyboardIsDown(OPKEY_D)){
		OPquat rotation = OPquatCreateRot((OPvec3*)&ship->frame.up, angle);
		*netRotation = OPquatMul(netRotation, &rotation);
	}
	if(OPkeyboardIsDown(OPKEY_W)){
		OPquat rotation = OPquatCreateRot((OPvec3*)&ship->frame.left, -angle);
		*netRotation = OPquatMul(netRotation, &rotation);
	}
	if(OPkeyboardIsDown(OPKEY_S)){
		OPquat rotation = OPquatCreateRot((OPvec3*)&ship->frame.left, angle);
		*netRotation = OPquatMul(netRotation, &rotation);
	}
	if(OPkeyboardIsDown(OPKEY_K)){
		OPquat rotation = OPquatCreateRot((OPvec3*)&ship->frame.forward, -angle);
		*netRotation = OPquatMul(netRotation, &rotation);
	}
	if(OPkeyboardIsDown(OPKEY_L)){
		OPquat rotation = OPquatCreateRot((OPvec3*)&ship->frame.forward, angle);
		*netRotation = OPquatMul(netRotation, &rotation);
	}

	if(OPkeyboardIsDown(OPKEY_ENTER)){
		ship->engine = 5.0f;
		ShipThrust(ship, OPvec2Zero, ship->engine, elapsedTime);
	}
	else{
		ship->engine -= ship->engine * 15 * elapsedTime;
	}
}
//-----------------------------------------------------------------------------
void playerGamepadControls(struct Ship* ship, OPfloat elapsedTime, OPquat* netRotation)
{
	if(!glfwJoystickPresent(0)) return;	// make sure the gamepad is connected

	OPfloat angle = 1.5f * elapsedTime; // define angle in respect to time
	int axes;
	const OPfloat* values = glfwGetJoystickAxes(0, &axes);

	OPquat yaw = OPquatCreateRot((OPvec3*)&ship->frame.up, OPpow(values[0], 3) * angle);
	OPquat pitch = OPquatCreateRot((OPvec3*)&ship->frame.left, -OPpow(values[3], 3) * angle);
	OPquat roll  = OPquatCreateRot((OPvec3*)&ship->frame.forward, OPpow(values[2], 3) * angle);

	*netRotation = OPquatMul(netRotation, &yaw);
	*netRotation = OPquatMul(netRotation, &pitch);
	*netRotation = OPquatMul(netRotation, &roll);

	ship->engine = -values[1];
	ShipThrust(ship, OPvec2Zero, ship->engine, elapsedTime);

	if(OPkeyboardIsDown(OPKEY_SPACE) || values[4] > 0.1){
		ShipFire(ship);
	}	
}
//-----------------------------------------------------------------------------
void playerHandler(struct Ship* ship, OPfloat elapsedTime)
{
	// query for the game camera's
	OPvec3 pos = OPcamGetPosition((&GAME_CAMERA));
	OPvec3 temp;

	OPvec3 jitter = OPvec3randNorm() * OPvec3len(&ship->velocity) * 0.05f;

	PLAYER_CAMERA_TARGET = ship->position + (ship->frame.forward * 2.5f) + (ship->frame.up * 0.5f) + jitter;
	pos += (PLAYER_CAMERA_TARGET - pos) / 10.0f;

	OPcamSetPosition((&GAME_CAMERA), pos);
	OPcamSetUp((&GAME_CAMERA), ship->frame.up);
	OPcamSetTarget((&GAME_CAMERA), ship->position);

	OPquat netRotation = OPquatIdentity;

	playerKeyboardControls(ship, elapsedTime, &netRotation);
	playerGamepadControls(ship, elapsedTime, &netRotation);

	ship->attitude = OPquatMul(&ship->attitude, &netRotation);
}
//-----------------------------------------------------------------------------
struct Controller InitControllerPlayer(struct Ship* ship)
{
	struct Controller controller = {
		ship,
		playerHandler
	};

	return controller;
}