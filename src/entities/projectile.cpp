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
OPeffect PROJECTILES_EFFECT;
OPmesh   PROJECTILES_QUAD;

//-----------------------------------------------------------------------------
//    ___      _ _   _      _ _         _   _          
//   |_ _|_ _ (_) |_(_)__ _| (_)_____ _| |_(_)___ _ _  
//    | || ' \| |  _| / _` | | |_ / _` |  _| / _ \ ' \ 
//   |___|_||_|_|\__|_\__,_|_|_/__\__,_|\__|_\___/_||_|
//               
OPint ProjectileInit()
{
	OPcmanLoad("Projectile.vert");
	OPcmanLoad("Projectile.frag");

	// compile and generate the effect used to render the projectiles
	PROJECTILES_EFFECT = OPrenderGenEffect(
		"Projectile.vert",
		"Projectile.frag",
		OPATTR_POSITION | OPATTR_UV,
		"Projectile Effect",
		0
	);

	PROJECTILES_QUAD = OPquadCreate();

	return 0;
}
//-----------------------------------------------------------------------------
OPint ProjectileDestroy()
{
	OPcmanUnload("Projectile.vert");
	OPcmanUnload("Projectile.frag");
	OPrenderUnloadEffect(&PROJECTILES_EFFECT);

	return 0;
}
//-----------------------------------------------------------------------------
struct Projectile* ProjectileSpawn(struct Projectile proj)
{
	// if we used up all the available projectiles (ie)
	// they are all flying around, then return early
	if(PROJECTILES_LIVING >= SPACE_PROJECTILES){
		return NULL;
	}

	proj.life = 2; // fly for 5 seconds
	PROJECTILES[PROJECTILES_LIVING] = proj; // add the projectile
	++PROJECTILES_LIVING;

	return PROJECTILES + PROJECTILES_LIVING - 1;
}

//-----------------------------------------------------------------------------
//    _   _          _      _         ___             _   _             
//   | | | |_ __  __| |__ _| |_ ___  | __|  _ _ _  __| |_(_)___ _ _  ___
//   | |_| | '_ \/ _` / _` |  _/ -_) | _| || | ' \/ _|  _| / _ \ ' \(_-<
//    \___/| .__/\__,_\__,_|\__\___| |_| \_,_|_||_\__|\__|_\___/_||_/__/
//         |_|              
void ProjectileBatchUpdate(OPfloat elapsedTime)
{
	for(register OPint i = PROJECTILES_LIVING; i--;){
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
			continue;
		}

		OPentHeap* targets;

		if((p->team & SHIP_TEAM_MASK) == SHIP_TEAM_RED){
			targets = SHIPS_BLUE;
		}
		else{
			targets = SHIPS_RED;
		}

		// check for collisions against ships
		for(register OPint j = targets->MaxIndex; j--;){
			OPentHeapIsLiving(targets, j); // this will skip ships that are not active (dead)

			struct Ship* target = ((struct Ship*)targets->Entities) + j;

			OPvec3 dif = p->position - target->position;
			if(target->hp > 0 && OPvec3dot(&dif, &dif) <= 1.0f){
				target->hp -= 0.5f;
				p->life = 0;
				break;
			}
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
	OPvec4 red  = { 1, 0, 0, 1 };
	OPvec4 blue = { 0, 0, 1, 1 };

	OPrenderBlendAdditive();
	OPrenderDepthWrite(0);
	OPbindMeshEffectWorldCam(&PROJECTILES_QUAD, &PROJECTILES_EFFECT, (OPmat4*)&OPmat4Identity, camera);

	for(register OPint i = PROJECTILES_LIVING; i--;){
		struct Projectile* p = PROJECTILES + i;
		
		OPrenderParamVec3("uPosition", &p->position);
		if(p->team == SHIP_TEAM_RED){
			OPrenderParamVec4("uColor", &red);
		}
		else{
			OPrenderParamVec4("uColor", &blue);
		}

		OPrenderMesh();
	}

	OPrenderDepthWrite(1);
	OPrenderBlendAlpha();
}