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
	OPtextureClearActive();
	OPbindMeshEffectWorldCam(SKY_MESH, &EFFECT_TEXTURED, (OPmat4*)&OPmat4Identity, camera);
	OPtextureBind(SKY_COLORMAP);
	OPrenderParami("uTexture", 0);
	OPrenderMesh();
}

void SkyDestroy()
{
	OPcmanUnload("skySphere.opm");
	OPcmanUnload("skySphere.png");
}