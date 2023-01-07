#include"init.h"
#include"defs.h"
#include"common.h"
#include"structs.h"
#include"main.h"
extern App app = { 0,0 };
extern void initSDL()
{
	int renderFlags, windowFlags;
	renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	windowFlags = SDL_WINDOW_BORDERLESS;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(fp, "Couldn't initialize SDL : %s\n", SDL_GetError());
		exit(1);
	}
	app.window = SDL_CreateWindow("D4SH | N30NSC4P3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

	if (!app.window)
	{
		fprintf(fp, "Failed to create a %d * %d SDL window", SCREEN_WIDTH, SCREEN_HEIGHT);
		exit(1);
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	app.renderer = SDL_CreateRenderer(app.window, -1, renderFlags);
	if (!app.renderer)
	{
		fprintf(fp, "Failed to create renderer : %s", SDL_GetError());
		exit(1);
	}
}