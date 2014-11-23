#include "./include/states/game.h"
#include "./include/entities/sky.h"

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

//    ___             _   _             
//   | __|  _ _ _  __| |_(_)___ _ _  ___
//   | _| || | ' \/ _|  _| / _ \ ' \(_-<
//   |_| \_,_|_||_\__|\__|_\___/_||_/__/
//                                      
void GSgameEnter(OPgameState* last)
{
	initSky();
	EFFECT_TEXTURED = OPeffectLoadTextured3D(sizeof(OPMvertexNormalUV));
	GAME_CAMERA = OPcamProj(
		OPvec3Zero,
		OPvec3Forward,
		OPvec3Up,
		0.01f,
		100.0f,
		90.0f,
		OPscreenWidthScale / OPscreenHeightScale
	);

	// clean up any old assets
	OPcmanPurge();
}

OPint GSgameUpdate(OPtimer* timer)
{
	float dt = OPtimerDelta(timer) / 1000.0f;

	gameDraw(dt);
	return 0;
}

void GSgameExit(OPgameState* target)
{
	destroySky();
}

void gameDraw(OPfloat dt)
{
	OPrenderClear(0.15, 0.15, 0.15);
	drawSky(&GAME_CAMERA);
	OPrenderPresent();
}