#include "./include/states/game.h"
#include "./include/entities/sky.h"
#include "./include/entities/ship.h"
#include "./include/entities/projectile.h"
#include "./include/controllers/player.h"
#include "./include/controllers/ai.h"

#define SPACE_AI_PLAYERS ((SHIP_MAX_TEAM_SIZE * 2) - 2)

//    ___                           _   ___         _               _   _             
//   | __|__ _ ___ __ ____ _ _ _ __| | |   \ ___ __| |__ _ _ _ __ _| |_(_)___ _ _  ___
//   | _/ _ \ '_\ V  V / _` | '_/ _` | | |) / -_) _| / _` | '_/ _` |  _| / _ \ ' \(_-<
//   |_|\___/_|  \_/\_/\__,_|_| \__,_| |___/\___\__|_\__,_|_| \__,_|\__|_\___/_||_/__/
//                                                                                    
void GSgameEnter(OPgameState* last);
OPint GSgameUpdate(OPtimer* timer);
void GSgameExit(OPgameState* target);

void gameDraw(OPfloat dt);

//    ___ _        _   _     __   __        _      _    _        
//   / __| |_ __ _| |_(_)__  \ \ / /_ _ _ _(_)__ _| |__| |___ ___
//   \__ \  _/ _` |  _| / _|  \ V / _` | '_| / _` | '_ \ / -_|_-<
//   |___/\__\__,_|\__|_\__|   \_/\__,_|_| |_\__,_|_.__/_\___/__/
//                                                               
OPgameState GS_GAME = {
	.Init   = GSgameEnter,
	.Update = GSgameUpdate,
	.Exit   = GSgameExit,
};

OPeffect EFFECT_TEXTURED;
OPcam GAME_CAMERA;

struct Controller CONTROLLER_PLAYER;
struct Controller CONTROLLER_AIS[SPACE_AI_PLAYERS];

//    ___             _   _             
//   | __|  _ _ _  __| |_(_)___ _ _  ___
//   | _| || | ' \/ _|  _| / _ \ ' \(_-<
//   |_| \_,_|_||_\__|\__|_\___/_||_/__/
//                                      
void GSgameEnter(OPgameState* last)
{
	SkyInit();
	ProjectileInit();
	ShipInit();

	// spawn a ship
	struct Ship* ship = ShipSpawn(SHIP_TEAM_BLUE);

	CONTROLLER_PLAYER = InitControllerPlayer(ship);

	for(OPint i = SPACE_AI_PLAYERS / 2; i--;){
		CONTROLLER_AIS[i * 2]     = InitControllerAI(ShipSpawn(SHIP_TEAM_BLUE));
		CONTROLLER_AIS[i * 2 + 1] = InitControllerAI(ShipSpawn(SHIP_TEAM_RED));
	}	


	OPvec3 pos = { 0, 0, -10 };
	EFFECT_TEXTURED = OPeffectLoadTextured3D(sizeof(OPMvertexNormalUV));

	// set up the game camera initially
	GAME_CAMERA = OPcamProj(
		pos,
		ship->position,
		OPvec3Up,
		0.01f,
		100.0f,
		90.0f,
		OPrenderWidth / (OPfloat)OPrenderHeight
	);

	// clean up any old assets
	OPcmanPurge();
}

OPint GSgameUpdate(OPtimer* timer)
{
	OPfloat dt = OPtimerDelta(timer);

	ShipBatchUpdate(dt);
	ProjectileBatchUpdate(dt);

	// update the player's controller
	CONTROLLER_PLAYER.handler(CONTROLLER_PLAYER.ship, dt);

	// now update all the ai
	for(register OPint i = SPACE_AI_PLAYERS; i--;){
		CONTROLLER_AIS[i].handler(CONTROLLER_AIS[i].ship, dt);
	}

	gameDraw(dt);
	return 0;
}

void GSgameExit(OPgameState* target)
{
	SkyDestroy();
	ProjectileDestroy();
	ShipDestroy();
}

void gameDraw(OPfloat dt)
{
	OPrenderClear(0.15, 0.15, 0.15);
	SkyDraw(&GAME_CAMERA);
	ShipBatchDraw(&GAME_CAMERA);
	ProjectileBatchDraw(&GAME_CAMERA);
	OPrenderPresent();
}