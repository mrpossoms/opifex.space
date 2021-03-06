#include "./Engine.h"
#include "./include/states/game.h"
#include "./include/entities/sky.h"

//////////////////////////////////////
// Application Methods
//////////////////////////////////////

void ApplicationInit() {

	OPchar* assetDir = NULL;
#ifdef OPIFEX_REPO
	assetDir = OPIFEX_REPO;
#endif
	OPloadersAddDefault();
	OPcmanInit(assetDir);

	OPaudInit();
	OPaudInitThread(100);

	// OPrenderFullscreen = 1;
	OPrenderSetScreenSize(800, 600);
	OPrenderInit();
	OPlog("Rendering initialized");
	OPsprite2DInit(NULL);
	OPlog("Sprite2D initialized");
	OPsprite3DInit(NULL);
	OPlog("Sprite3D initialized");
	OPsystemsLoadFontEffect();
	OPgamePadSetDeadzones(0.1f);
	OPgameStateChange(&GS_GAME);
}

int ApplicationUpdate(OPtimer* timer) {
	
	OPcmanUpdate();
	OPgamePadSystemUpdate();
	OPinputSystemUpdate();

	if(timer->Elapsed < 32){
		usleep(10000);
	}
	
	return ActiveState->Update(timer);
}

void ApplicationDestroy() {
	ActiveState->Exit(ActiveState);
}

void ApplicationSetup() {
	OPinitialize = ApplicationInit;
	OPupdate = ApplicationUpdate;
	OPdestroy = ApplicationDestroy;
}

//////////////////////////////////////
// Application Entry Point
//////////////////////////////////////
OP_MAIN {
	OPlog("Starting up OPifex Engine");

	OPinitialize = ApplicationInit;
	OPupdate = ApplicationUpdate;
	OPdestroy = ApplicationDestroy;

	OP_MAIN_START
	OP_MAIN_END
	OP_MAIN_SUCCESS
}