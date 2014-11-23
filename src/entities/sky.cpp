#include "include/Entities/sky.h"

// asset proxies
OPmesh*    meshSkySphere;
OPtexture* textureSkySphere;


void initSky()
{
	OPcmanLoad("skySphere.opm");
	OPcmanLoad("skySphere.png");
	
	// assign proxy variables to their assets in the content manager
	meshSkySphere    = (OPmesh*)OPcmanGet("skySphere.opm");
	textureSkySphere = (OPtexture*)OPcmanGet("skySphere.png");
}

void drawSky(OPcam* camera)
{
	OPtextureClearActive();
	OPbindMeshEffectWorldCam(meshSkySphere, &EFFECT_TEXTURED, (OPmat4*)&OPmat4Identity, camera);
	OPtextureBind(textureSkySphere);
	OPrenderParami("uTexture", 0);
	OPrenderMesh();
}

void destroySky()
{
	OPcmanUnload("skySphere.opm");
	OPcmanUnload("skySphere.png");
}