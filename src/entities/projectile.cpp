#include "./include/entities/projectile.h"

//-----------------------------------------------------------------------------
//     ___             _            _      
//    / __|___ _ _  __| |_ __ _ _ _| |_ ___
//   | (__/ _ \ ' \(_-<  _/ _` | ' \  _(_-<
//    \___\___/_||_/__/\__\__,_|_||_\__/__/
//                                         
#define SPACE_PROJECTILES 500

//-----------------------------------------------------------------------------
//    __  __                    
//   |  \/  |__ _ __ _ _ ___ ___
//   | |\/| / _` / _| '_/ _ (_-<
//   |_|  |_\__,_\__|_| \___/__/
//                              
#define PROJ_KILL_AND_SWAP(i){\
	--PROJECTILES_LIVING;\
	PROJECTILES[i] = PROJECTILES[PROJECTILES_LIVING];\
}\

//-----------------------------------------------------------------------------
//    ___ _        _   _     __   __           
//   / __| |_ __ _| |_(_)__  \ \ / /_ _ _ _ ___
//   \__ \  _/ _` |  _| / _|  \ V / _` | '_(_-<
//   |___/\__\__,_|\__|_\__|   \_/\__,_|_| /__/
//                                             
struct Projectile PROJECTILES[SPACE_PROJECTILES];
OPint PROJECTILES_LIVING;

//-----------------------------------------------------------------------------
//    ___      _ _   _      _ _         _   _          
//   |_ _|_ _ (_) |_(_)__ _| (_)_____ _| |_(_)___ _ _  
//    | || ' \| |  _| / _` | | |_ / _` |  _| / _ \ ' \ 
//   |___|_||_|_|\__|_\__,_|_|_/__\__,_|\__|_\___/_||_|
//               
OPint ProjectileInit()
{

}

OPint ProjectileDestroy()
{

}

struct Projectile* ProjectileSpawn(struct Projectile proj)
{
	// if we used up all the available projectiles (ie)
	// they are all flying around, then return early
	if(PROJECTILES_LIVING >= SPACE_PROJECTILES){
		return NULL;
	}

	proj.life = 5; // fly for 5 seconds
}

//-----------------------------------------------------------------------------
//    _   _          _      _         ___             _   _             
//   | | | |_ __  __| |__ _| |_ ___  | __|  _ _ _  __| |_(_)___ _ _  ___
//   | |_| | '_ \/ _` / _` |  _/ -_) | _| || | ' \/ _|  _| / _ \ ' \(_-<
//    \___/| .__/\__,_\__,_|\__\___| |_| \_,_|_||_\__|\__|_\___/_||_/__/
//         |_|              
void ProjectileBatchUpdate(OPfloat elapsedTime)
{
	for(OPint i = PROJECTILES_LIVING; i--;){
		// proxy variable for the current projectile
		struct Projectile* p = PROJECTILES + i;

		// update the life and position
		p->life -= elapsedTime;
		p->position += p->velocity * elapsedTime;

		// He's dead Jim...
		if(p->life <= 0){
			// swap the last living proj with the one that just
			// died. This keeps living projectiles contiguous	
			PROJ_KILL_AND_SWAP(i);
		}
	}
}

//-----------------------------------------------------------------------------
//    ___                  _             ___             _   _             
//   |   \ _ _ __ ___ __ _(_)_ _  __ _  | __|  _ _ _  __| |_(_)___ _ _  ___
//   | |) | '_/ _` \ V  V / | ' \/ _` | | _| || | ' \/ _|  _| / _ \ ' \(_-<
//   |___/|_| \__,_|\_/\_/|_|_||_\__, | |_| \_,_|_||_\__|\__|_\___/_||_/__/
//                               |___/                                     
void ProjectileBatchDraw(OPcam* camera)
{
	for(OPint i = PROJECTILES_LIVING; i--;){

	}
}