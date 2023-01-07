#include"common.h"
#include"init.h"
#include"draw.h"
#include"input.h"
#include"mouse_widgets.h"
#include"defs.h"
#include<time.h>
extern void (*loadSceneFunc)(void) = drawInitScene;
extern void (*updatePlayerFunc)(void) = updatePlayer;
extern void (*updateSceneFunc)(void) = NULL;
extern FILE* fp = NULL, * scoref = NULL;
extern TTF_Font* font = NULL;
int WinMain()
{
	//initialize debug output and highscore
	PLAYER_STATE = READY;
	clock_t start_t = 0, end_t = 0;
	int deltatime_ms = 0;
	int del = 0;
#ifdef LOG_ON
	fp = fopen("debug.txt", "w");
#endif
	scoref = fopen("data/score.dat", "r");
	if (scoref)
	{
		fscanf(scoref, "%d", &last_highscore);
#ifdef LOG_ON
		fprintf(fp, "highscore read\n");
#endif
		fclose(scoref);
	}
	scoref = fopen("data/score.dat", "w");
	srand(time(0));

	//init SDL and game mechanics
	memset(&app, 0, sizeof(App));
	initSDL();
	TTF_Init();
	initButtonPool();
	initLayerManager();
	initTimerManager();
	initResourceManager();
	SDL_StopTextInput();

	//load resources (pictures)
	//loadResource("pictures/Logo.png", 0, 1, "logo");
	loadResource("pictures/logo/", 1, 20, "logo");
	loadResource("pictures/Starticon.png", 0, 1, "start_icon");
	loadResource("pictures/instructions.png", 0, 1, "instructions");
	loadResource("pictures/blackback.png", 0, 1, "blackback");
	loadResource("pictures/road.png", 0, 1, "road");
	loadResource("pictures/player/", 1, 8, "player-run");
	loadResource("pictures/player/duck.png", 0, 1, "player-duck");
	loadResource("pictures/player/jump.png", 0, 1, "player-jump");
	loadResource("pictures/block_1/", 1, 15, "block_1");
	loadResource("pictures/block_2/", 1, 1, "board-block-fore");
	loadResource("pictures/block_2/block2_back.png", 0, 1, "board-block-back");
	loadResource("pictures/block_2/block2_beam.png", 0, 1, "board-beam");
	loadResource("pictures/light/light_back.png", 0, 1, "light-back");
	loadResource("pictures/light/light_beam.png", 0, 1, "light-beam");
	loadResource("pictures/player/fail/", 1, 2, "player-fail");
	loadResource("pictures/failscreen.png", 0, 1, "failscreen");
	loadResource("pictures/restarticon.png", 0, 1, "restart_icon");
	loadResource("pictures/buildings/building_1.png", 0, 1, "building_1");
	loadResource("pictures/buildings/building_2.png", 0, 1, "building_2");
	loadResource("pictures/buildings/building_3.png", 0, 1, "building_3");
	loadResource("pictures/buildings/background/backbuild_1.png", 0, 1, "backbuild_1");
	loadResource("pictures/buildings/background/backbuild_2.png", 0, 1, "backbuild_2");
	loadResource("pictures/buildings/background/backbuild_3.png", 0, 1, "backbuild_3");
	loadResource("pictures/buildings/background.png", 0, 1, "city_back");
	loadResource("pictures/player/swing/", 1, 4, "player-swing");
	loadResource("pictures/props/wings.png", 0, 1, "props-wings");
	font = TTF_OpenFont("fonts/SmileySans-Oblique.ttf", 30);

	//init and play music
	//place your own bgm.mp3 in the music folder
	/*
	Mix_Init(MIX_INIT_MP3);
	Mix_OpenAudio(48000, AUDIO_S16SYS, 2, 2048);
	Mix_Music* bgm = Mix_LoadMUS("music/bgm.mp3");
	Mix_VolumeMusic((int)((double)0.4 * MIX_MAX_VOLUME));
	Mix_PlayMusic(bgm, -1);
	*/

	//main loop
	
	loadSceneFunc();
	while (1)
	{
		del = 0;
		//these clock()s are for testing the performance of each function in the loop, measured in ms

		start_t = clock();

		prepareScene();

		end_t = clock();
		deltatime_ms = end_t - start_t;
		del = MAX(del, deltatime_ms);
#ifdef LOG_ON
		fprintf(fp, "operation preparescene took %d ms\n", deltatime_ms);
#endif


		start_t = clock();

		updatePlayerFunc();

		end_t = clock();
		deltatime_ms = end_t - start_t;
		del = MAX(del, deltatime_ms);
#ifdef LOG_ON
		fprintf(fp, "operation updatePlayer took %d ms\n", deltatime_ms);
#endif

		//after series of debug i confirmed that the hotspot was in this function. other functions took ~0ms.
		start_t = clock();

		updateTimerManager();

		end_t = clock();
		deltatime_ms = end_t - start_t;
		del = MAX(del, deltatime_ms);
#ifdef LOG_ON
		fprintf(fp, "operation updateTimers took %d ms\n", deltatime_ms);
#endif

		start_t = clock();

		updateTotalEntities();

		end_t = clock();
		deltatime_ms = end_t - start_t;
		del = MAX(del, deltatime_ms);
#ifdef LOG_ON
		fprintf(fp, "operation updateTotalEntities took %d ms\n", deltatime_ms);

		fprintf(fp, "current entities: %d\n", total_Entities);
#endif
		start_t = clock();

		updateLayers();

		end_t = clock();
		deltatime_ms = end_t - start_t;
		del = MAX(del, deltatime_ms);
#ifdef LOG_ON
		fprintf(fp, "operation updateLayers took %d ms\n", deltatime_ms);
#endif
		start_t = clock();

		if (updateSceneFunc) updateSceneFunc();

		end_t = clock();
		deltatime_ms = end_t - start_t;
		del = MAX(del, deltatime_ms);
#ifdef LOG_ON
		fprintf(fp, "operation updateMainScene took %d ms\n", deltatime_ms);
#endif
		start_t = clock();

		if (PLAYER_STATE == NORMAL || PLAYER_STATE == DUCKING || PLAYER_STATE == SUSPENDED)
		{
			updateCollision("Blockages");
			updateCollision("Board-Blockage-Fore");
			updateCollision("props");
		}
#ifdef LOG_ON
		fprintf(fp, "player state is %d\n", PLAYER_STATE);
#endif
		

		end_t = clock();
		deltatime_ms = end_t - start_t;
		del = MAX(del, deltatime_ms);
#ifdef LOG_ON
		fprintf(fp, "operation updateCollision took %d ms\n", deltatime_ms);
#endif
		updateButtonPool();
		doInput();
		presentScene();


		//this is to prevent the sudden changes of framerate if a function took too much time to work (>= 1ms)
#ifdef LOG_ON
		fprintf(fp, "delta is %d\n", del);
#endif
		if (del >= 16)del = 0;
		if (del < 0)del = 0;
#ifdef LOG_ON
		fprintf(fp, "deltatime is %d ms\n", del);
#endif
		SDL_Delay(DELTA_TIME - del);
		if (PLAYER_STATE != READY && !IS_PAUSED && PLAYER_STATE != FAILED)
		{
			currentTick++;
			updateSpeedFactor();
#ifdef LOG_ON
			fprintf(fp, "speedFactor is %lf\n", speedFactor);
#endif
		}
	}
	return 0;
}