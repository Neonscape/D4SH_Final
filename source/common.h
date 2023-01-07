#pragma once
#include<SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include"defs.h"
#include"structs.h"
#include<time.h>
#include<SDL_ttf.h>
#include<math.h>
extern int total_Entities;
extern int IS_PAUSED;
extern int last_highscore, current_score;
extern int getIntLength(int);
extern char* intToString(int);
extern int getRelPosX(double);
extern int getRelPosY(double);
extern void loadEntity2D(Entity2D* entity, double x, double y, const char* tag, const char* restag);
extern SDL_Rect getRectFromTex(SDL_Texture* tex, int x, int y);
extern SDL_Rect getRectFromEntity2D(Entity2D* entity);
extern void updateEntity2D(Entity2D* entity);
extern void drawEntity2D(Entity2D* entity);
extern void initEntity2DLayer(Entity2DLayer*);
extern void initEntity2D(Entity2D*);
extern Entity2D* addEntity2D(Entity2DLayer*);
extern void freeEntity2DLayer(Entity2DLayer*);
extern void setEntity2DSpeed(Entity2D*, double, double, double, double);
extern int isVisible(Entity2D*);
extern int getEntityWidth(Entity2D*);
extern int getEntityHeight(Entity2D*);
extern void freeEntity2D(Entity2D*);
extern void updateEntity2DLayer(Entity2DLayer*);
extern void initAnimator(Animator*);
extern void initAnimatorLayer(AnimatorLayer*);
extern Animator* addAnimator(AnimatorLayer*);
extern void freeAnimator(Animator*);
extern void updateAnimator(Animator*);
extern void freeAnimatorLayer(AnimatorLayer*);
extern void setAnimator(Animator* animator, int totalFrames, int framerate, double relx, double rely,  const char* tag, const char* restag);
extern void updateAnimatorLayer(AnimatorLayer*);
extern void initLayerManager();
extern Entity2DLayer* addEntity2DLayer(int, const char*);
extern AnimatorLayer* addAnimatorLayer(int, const char*);
extern void* getLayerByIndex(int);
extern void* getLayerByTag(const char*);
extern void updateLayers();
extern void removeLayer(int);
extern Timer* addTimer();
extern void initTimer(Timer*);
extern void initTimerManager();
extern void setTimer(Timer*, int, void(*)(void), const char*);
extern void freeTimer(Timer*);
extern void updateTimer(Timer*);
extern void updateTimerManager();
extern Timer* getTimerByTag(const char*);
extern Animator* getAnimatorByTag(AnimatorLayer*, const char*);
extern void updateTotalEntities();
extern void setPlayerEntity2DSpeed(double velx, double vely, double accex, double accey);;
extern void setEntity2DCollideBox(Entity2D* entity, int w, int h, int disx, int disy);
extern int isCollided(Entity2D*, Entity2D*);
extern void updateCollision(const char* tag);
extern void changeAnimatorTexture(Animator* animator, SDL_Texture** texlist, int totalFrames, int framerate);
extern void initResourceManager();
extern void loadResource(const char* path, int type, int totalTex, const char* tag);
extern SDL_Texture** getResourceByTag(const char* tag);
extern void freeAllLayers();
extern void updateSpeedFactor();