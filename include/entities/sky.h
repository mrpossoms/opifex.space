#ifndef SPACE_SKY
#define SPACE_SKY

#include "./Engine.h"
#include "./include/states/game.h"

//-----------------------------------------------------------------------------
//    ___         _       _                     
//   | _ \_ _ ___| |_ ___| |_ _  _ _ __  ___ ___
//   |  _/ '_/ _ \  _/ _ \  _| || | '_ \/ -_|_-<
//   |_| |_| \___/\__\___/\__|\_, | .__/\___/__/
//                            |__/|_|           
void SkyInit();
void SkyDraw(OPcam* camera);
void SkyDestroy();

#endif