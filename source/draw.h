#pragma once
#include"common.h"
extern void prepareScene();
extern void presentScene();
extern void drawInitScene();
extern void drawMainScene();
extern void drawRoad();
extern void drawBlockages();
extern void drawLight();
extern void drawBuildings();
extern void drawBackBuildings();
extern void drawBackground();
extern void switchToMainScene();
extern void updatePlayer();
extern void updateMainScene();
extern void updatePlayerFail();
extern void deactivateInvincible();
extern void recoverPlayerStateFromProp();