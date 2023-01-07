#pragma once
#include"common.h"
typedef struct
{
	SDL_Renderer* renderer;
	SDL_Window* window;
}App;
typedef struct
{
	double x, y;		//coordinates
	int collh, collw;	//width and height of the collide box.
	int disx, disy;		//displacement of the collide box from the origin of an entity(0,0).
	double velX, velY;	//velocity
	double acceleX, acceleY;
	int id;
	SDL_Texture* tex;
	char* tag;
}Entity2D;
typedef struct
{
	Entity2D* layer;
	int cnt;
	int freeptr[MEMORY_ALLOC];
	int cnt2;
	char* tag;
	int layerid;
	char* type;
}Entity2DLayer;
typedef struct
{
	Entity2D entity;
	SDL_Texture** texList;
	int totalFrames;
	int framerate;
	int framecount;
	int currentFrame;
	int layerid;
	char* tag;
	char* type;
}Animator;
typedef struct
{
	Animator* layer;
	int cnt, cnt2;
	char* tag;
	int layerid;
	int freeptr[MEMORY_ALLOC];
	char* type;
}AnimatorLayer;
typedef struct
{
	void** layerPtr;
	int layerCount;
	char* type;
}LayerManager;
typedef struct
{
	int currCount;
	int loopCount;
	int id;
	void (*func)(void);
	char* tag;
}Timer;
typedef struct
{
	Timer* timerList;
	int cnt;
	int freeptr[MEMORY_ALLOC];
	int cnt2;
}TimerManager;
typedef struct
{
	SDL_Texture** texList;
	int texCount;
	char* tag;
}Resource;
typedef struct
{
	Resource* resList;
	int totalRes;
}ResourceManager;
extern double speedFactor;
extern int currentTick;