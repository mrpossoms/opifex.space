#include "include/entities/sky.h"

// asset proxies
OPmesh*    SKY_MESH;
OPtexture* SKY_COLORMAP;

//-----------------------------------------------------------------------------
//    ___      _ _   _      _ _         _   _          
//   |_ _|_ _ (_) |_(_)__ _| (_)_____ _| |_(_)___ _ _  
//    | || ' \| |  _| / _` | | |_ / _` |  _| / _ \ ' \ 
//   |___|_||_|_|\__|_\__,_|_|_/__\__,_|\__|_\___/_||_|
//               
void SkyInit()
{
	OPcmanLoad("skySphere.opm");
	OPcmanLoad("skySphere.png");
	
	// assign proxy variables to their assets in the content manager
	SKY_MESH     = (OPmesh*)OPcmanGet("skySphere.opm");
	SKY_COLORMAP = (OPtexture*)OPcmanGet("skySphere.png");
}
//-----------------------------------------------------------------------------
void SkyDestroy()
{
	OPcmanUnload("skySphere.opm");
	OPcmanUnload("skySphere.png");
}

//-----------------------------------------------------------------------------
//    ___                  _             ___             _   _             
//   |   \ _ _ __ ___ __ _(_)_ _  __ _  | __|  _ _ _  __| |_(_)___ _ _  ___
//   | |) | '_/ _` \ V  V / | ' \/ _` | | _| || | ' \/ _|  _| / _ \ ' \(_-<
//   |___/|_| \__,_|\_/\_/|_|_||_\__, | |_| \_,_|_||_\__|\__|_\___/_||_/__/
//                               |___/                               
void SkyDraw(OPcam* camera)
{
	OPmat4 world = OPmat4createScl(100, 100, 100);
	OPmat4translate(&world, &OPcamGetPosition(camera));

	OPrenderDepth(0);
	OPtextureClearActive();
	OPbindMeshEffectWorldCam(SKY_MESH, &EFFECT_TEXTURED, &world, camera);
	OPtextureBind(SKY_COLORMAP);
	OPrenderParami("uTexture", 0);
	OPrenderMesh();
	OPrenderDepth(1);
}