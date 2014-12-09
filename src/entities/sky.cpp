#include "include/entities/sky.h"

// asset proxies
OPmesh*    SKY_MESH;
OPtexture* SKY_COLORMAP;


void SkyInit()
{
	OPcmanLoad("skySphere.opm");
	OPcmanLoad("skySphere.png");
	
	// assign proxy variables to their assets in the content manager
	SKY_MESH     = (OPmesh*)OPcmanGet("skySphere.opm");
	SKY_COLORMAP = (OPtexture*)OPcmanGet("skySphere.png");
}

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

void SkyDestroy()
{
	OPcmanUnload("skySphere.opm");
	OPcmanUnload("skySphere.png");
}