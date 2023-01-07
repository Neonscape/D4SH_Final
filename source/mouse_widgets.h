#pragma once
#include"common.h"
#include"structs.h"
typedef struct
{
	Entity2D entity;
	void (*func)(void);
}Button;
typedef struct
{
	Button* layer;
	int cnt;
	int freeptr[MEMORY_ALLOC];
	int cnt2;
}ButtonLayer;
extern void initButton(Button*);
extern void initButtonPool();
extern Button* addButton();
extern void delButton(Button*);
extern void checkButton(Button, SDL_MouseButtonEvent);
extern void updateButtonPool();
extern void checkButtonPool(SDL_MouseButtonEvent);
extern void freeButtonPool();