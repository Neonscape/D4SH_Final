#pragma once
#include"common.h"
extern void (*loadSceneFunc)(void);//the pointer to the function to switch the scene
extern void (*updatePlayerFunc)(void);
extern void (*updateSceneFunc)(void);
extern FILE* fp, *scoref;
extern TTF_Font* font;