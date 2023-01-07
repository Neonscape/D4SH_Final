#include"common.h"
#include"init.h"
#include"main.h"
#include"input.h"
#include"mouse_widgets.h"
extern void prepareScene()
{
	SDL_SetRenderDrawColor(app.renderer, 26, 16, 3, 200);
	SDL_RenderClear(app.renderer);
}
extern void presentScene()
{
	SDL_RenderPresent(app.renderer);
}
extern void drawRoad()
{
	if (PLAYER_STATE == FAILED || IS_PAUSED)return;
	Entity2DLayer* roadlayer = getLayerByTag("Main-Road");
	Entity2D* road = addEntity2D(roadlayer);
	loadEntity2D(road, 0.99, 0.78, "road", "road");
	setEntity2DSpeed(road, -10 * speedFactor, 0, 0, 0);
	Timer* roadTimer = getTimerByTag("road");
	setTimer(roadTimer, (int)((500.0 / 10.0 / speedFactor) - 3), drawRoad, "road");
}
extern void drawBlockages()
{
	if (PLAYER_STATE == FAILED || IS_PAUSED)return;
	Timer* timer = getTimerByTag("blockage-timer");
	int id = timer->id;
	initTimer(timer);
	timer->id = id;
	int del = ((double)rand() / (double)RAND_MAX) * 50;
	setTimer(timer, (int)((50.0 / speedFactor) - 3) + del, drawBlockages, "blockage-timer");
	int factor2 = rand();
	if (factor2 & 1)
	{
		clock_t start = clock();
		AnimatorLayer* layer = getLayerByTag("Blockages");
		if (!layer)return;
#ifdef LOG_ON
		fprintf(fp, "Blockages layer caught\n");
#endif
		Animator* newBlockage = addAnimator(layer);
		double factor = ((double)(rand() - RAND_MAX / 2) / (double)(RAND_MAX / 2)) * 0.03;
#ifdef LOG_ON
		fprintf(fp, "factor is %lf\n", factor);
#endif
		setAnimator(newBlockage, 15, 2, 0.99, 0.81 + factor, "block_1", "block_1");
		setEntity2DCollideBox(&newBlockage->entity, 30, 90, 20, 30);
		setEntity2DSpeed(&newBlockage->entity, -10 * speedFactor, 0, 0, 0);
		clock_t end = clock();
#ifdef LOG_ON
		fprintf(fp, "draw block_1 took %dms\n", end - start);
#endif
	}
	else
	{
		clock_t start = clock();
		AnimatorLayer* forelayer = getLayerByTag("Board-Blockage-Fore");
		if (!forelayer)return;
		Animator* newBlock = addAnimator(forelayer);
		if (!newBlock)return;
		setAnimator(newBlock, 1, 0, 0.99, 0.55,"board-block-fore", "board-block-fore");
		setEntity2DCollideBox(&newBlock->entity, 100, 150, 70, 30);
		setEntity2DSpeed(&newBlock->entity, -10 * speedFactor, 0, 0, 0);
		Entity2DLayer* backlayer = getLayerByTag("Board-Blockage-Back");
		if (!backlayer)return;
		Entity2D* newBack = addEntity2D(backlayer);
		if (!newBack)return;
		loadEntity2D(newBack, 0.99, 0.55,"board-block-back", "board-block-back");
		setEntity2DSpeed(newBack, -10 * speedFactor, 0, 0, 0);
		Entity2DLayer* lightLayer = getLayerByTag("streetlight_beam");
		if (!lightLayer)return;
		Entity2D* newLight = addEntity2D(lightLayer);
		if (!lightLayer)return;
		loadEntity2D(newLight, 0.99, 0.55, "board-beam", "board-beam");
		setEntity2DSpeed(newLight, -10 * speedFactor, 0, 0, 0);
		clock_t end = clock();
#ifdef LOG_ON
		fprintf(fp, "draw block 2 took %dms\n", end - start);
#endif
	}
	
	
}
extern void drawLight()
{
	if (PLAYER_STATE == FAILED || IS_PAUSED)return;
	Entity2DLayer* lightLayer = getLayerByTag("streetlight_beam");
	Entity2DLayer* backLayer = getLayerByTag("streetlight_back");
	if (lightLayer == NULL || backLayer == NULL)return;
	Entity2D* newLight = addEntity2D(lightLayer);
	Entity2D* newLightBack = addEntity2D(backLayer);
	loadEntity2D(newLight, 0.99, 0.15, "light-beam", "light-beam");
	loadEntity2D(newLightBack, 0.99, 0.15, "light-back", "light-back");
	setEntity2DSpeed(newLight, -10 * speedFactor, 0, 0, 0);
	setEntity2DSpeed(newLightBack, -10 * speedFactor, 0, 0, 0);
	Timer* lightTimer = getTimerByTag("drawLight");
	setTimer(lightTimer, (int)((100.0 / speedFactor) - 3), drawLight, "drawLight");
}
extern void drawBuildings()
{
	if (PLAYER_STATE == FAILED || IS_PAUSED)return;
	Timer* buildingTimer = getTimerByTag("drawBuildings");
	if (!buildingTimer)return;
	int factor = (rand() / (RAND_MAX / 50));
	setTimer(buildingTimer, (int)((70.0 / speedFactor) - 3) + factor, drawBuildings, "drawBuildings");
#ifdef LOG_ON
	fprintf(fp, "factor is %d\n", factor);
#endif
	if (PLAYER_STATE == FAILED || IS_PAUSED)return;
	int rangen = rand();
	Entity2DLayer* buildLayer = getLayerByTag("Buildings");
	if (!buildLayer)return;
	Entity2D* newBuilding = addEntity2D(buildLayer);
	if (!newBuilding)return;
	if (rangen <= 20000)
	{
		loadEntity2D(newBuilding, 0.99, 0.25, "build-1", "building_1");
		setEntity2DSpeed(newBuilding, -10 * speedFactor, 0, 0, 0);
		return;
	}
	if (WITHIN(20001, 25000, rangen))
	{
		loadEntity2D(newBuilding, 0.99, 0.27, "build-2", "building_2");
		setEntity2DSpeed(newBuilding, -10 * speedFactor, 0, 0, 0);
		return;
	}
	loadEntity2D(newBuilding, 0.99, 0.23, "build-3", "building_3");
	setEntity2DSpeed(newBuilding, -10 * speedFactor, 0, 0, 0);
}
extern void drawBackBuildings()
{
	Timer* backTimer = getTimerByTag("drawBackBuildings");
	if (!backTimer)return;
	int factor = (rand() / (RAND_MAX / 20));
	setTimer(backTimer, (int)((70.0 / speedFactor) - 3) + factor, drawBackBuildings, "drawBackBuildings");
	Entity2DLayer* backlayer = getLayerByTag("BackBuildings");
	if (!backlayer)return;
	Entity2D* newBack = addEntity2D(backlayer);
	if (!newBack)return;
	int factor2 = rand();
	if (factor2 < 10000)
	{
		loadEntity2D(newBack, 0.99, 0.11, "backbuild-1", "backbuild_1");
		setEntity2DSpeed(newBack, -5 * speedFactor, 0, 0, 0);
		return;
	}
	if (WITHIN(10001, 17000, factor2))
	{
		loadEntity2D(newBack, 0.99, 0.11, "backbuild-2", "backbuild_2");
		setEntity2DSpeed(newBack, -5 * speedFactor, 0, 0, 0);
		return;
	}
	loadEntity2D(newBack, 0.99, 0.11, "backbuild-3", "backbuild_3");
	setEntity2DSpeed(newBack, -5 * speedFactor, 0, 0, 0);
	return;

}
extern void drawBackground()
{
	Entity2DLayer* bgLayer = getLayerByTag("city-background");
	if (!bgLayer)return;
	Entity2D* newbg = addEntity2D(bgLayer);
	if (!newbg)return;
	loadEntity2D(newbg, 0.999, 0, "bg", "city_back");
	setEntity2DSpeed(newbg, -2 * speedFactor, 0, 0, 0);
	Timer* bgTimer = getTimerByTag("drawbg");
	setTimer(bgTimer, (int)((1600.0 / 2.0 / speedFactor) - 3), drawBackground, "drawbg");
}
extern void drawProps()
{
	if (PLAYER_STATE == FLYING)return;
	Timer* propsTimer = getTimerByTag("drawProps");
	int factor = rand() / 30 + 1000;
	setTimer(propsTimer, factor, drawProps, "drawProps");
	AnimatorLayer* propsLayer = getLayerByTag("props");
	if (!propsLayer)return;
	Animator* newProp = addAnimator(propsLayer);
	setAnimator(newProp, 1, 100, 0.99, 0.56, "wings", "props-wings");
	setEntity2DSpeed(&newProp->entity, -10 * speedFactor, 0, 0, 0);
	setEntity2DCollideBox(&newProp->entity, 170, 100, 15, 15);
}
extern void drawMainScene()
{
	speedFactor = 0;
	currentTick = 0;
	PLAYER_STATE = NORMAL;
	AnimatorLayer* anilayer = addAnimatorLayer(11, "Main-Player");
	AnimatorLayer* block2layer = addAnimatorLayer(15, "Board-Blockage-Fore");
	Entity2DLayer* failLayer = addEntity2DLayer(20, "Fail");
	Animator* player = addAnimator(anilayer);
	Entity2DLayer* entlayer = addEntity2DLayer(7, "Main-Road");
	AnimatorLayer* anilayer_2 = addAnimatorLayer(8, "Blockages");
	Entity2DLayer* entlayer_3 = addEntity2DLayer(9, "Board-Blockage-Back");
	Entity2DLayer* buildingLayer = addEntity2DLayer(5, "Buildings");
	Entity2DLayer* backbuildinglayer = addEntity2DLayer(4, "BackBuildings");
	Entity2DLayer* backgroundLayer = addEntity2DLayer(2, "city-background");
	AnimatorLayer* propsLayer = addAnimatorLayer(17, "props");
	setAnimator(player, 8, 4, 0.2, 0.7, "player", "player-run");
	setEntity2DCollideBox(&player->entity, 70, 170, 15, 0);
	//setEntity2DSpeed(&player->entity, 0, -10, 0, 0);


	Entity2D* road = addEntity2D(entlayer);
	loadEntity2D(road, 0, 0.78, "road", "road");
	setEntity2DSpeed(road, -10, 0, 0, 0);
	road = addEntity2D(entlayer);
	loadEntity2D(road, 0.2, 0.78, "road", "road");
	setEntity2DSpeed(road, -10, 0, 0, 0);
	road = addEntity2D(entlayer);
	loadEntity2D(road, 0.4, 0.78, "road", "road");
	setEntity2DSpeed(road, -10, 0, 0, 0);
	road = addEntity2D(entlayer);
	loadEntity2D(road, 0.6, 0.78, "road", "road");
	setEntity2DSpeed(road, -10, 0, 0, 0);
	road = addEntity2D(entlayer);
	loadEntity2D(road, 0.8, 0.78, "road", "road");
	setEntity2DSpeed(road, -10, 0, 0, 0);
	road = addEntity2D(entlayer);
	loadEntity2D(road, 0.99, 0.78, "road", "road");
	setEntity2DSpeed(road, -10, 0, 0, 0);
	//setEntity2DSpeed(&player->entity, 4, 0, 0, 0);
	
	Entity2D* building = addEntity2D(buildingLayer);
	loadEntity2D(building, 0.05, 0.25, "build-1", "building_1");
	setEntity2DSpeed(building, -10, 0, 0, 0);
	building = addEntity2D(buildingLayer);
	loadEntity2D(building, 0.45, 0.27, "build-2", "building_2");
	setEntity2DSpeed(building, -10, 0, 0, 0);
	building = addEntity2D(buildingLayer);
	loadEntity2D(building, 0.9, 0.23, "build-3", "building_3");
	setEntity2DSpeed(building, -10, 0, 0, 0);
	//Animator* blockage = addAnimator(anilayer_2);
	//setAnimator(blockage, "pictures/block_1/", 15, 2, 0.7, 0.8, "blockage");
	Entity2D* backBuild = addEntity2D(backbuildinglayer);
	loadEntity2D(backBuild, 0.1, 0.11, "backbuild-1", "backbuild_1");
	setEntity2DSpeed(backBuild, -5, 0, 0, 0);
	backBuild = addEntity2D(backbuildinglayer);
	loadEntity2D(backBuild, 0.3, 0.11, "backbuild-2", "backbuild_2");
	setEntity2DSpeed(backBuild, -5, 0, 0, 0);
	backBuild = addEntity2D(backbuildinglayer);
	loadEntity2D(backBuild, 0.55, 0.11, "backbuild-3", "backbuild_3");
	setEntity2DSpeed(backBuild, -5, 0, 0, 0);
	backBuild = addEntity2D(backbuildinglayer);
	loadEntity2D(backBuild, 0.75, 0.11, "backbuild-1", "backbuild_1");
	setEntity2DSpeed(backBuild, -5, 0, 0, 0);
	backBuild = addEntity2D(backbuildinglayer);
	loadEntity2D(backBuild, 0.95, 0.11, "backbuild-3", "backbuild_3");
	setEntity2DSpeed(backBuild, -5, 0, 0, 0);
	Entity2D* bg = addEntity2D(backgroundLayer);
	loadEntity2D(bg, 0, 0, "city-bg", "city_back");
	setEntity2DSpeed(bg, -2, 0, 0, 0);
	bg = addEntity2D(backgroundLayer);
	loadEntity2D(bg, 0.999, 0, "city-bg", "city_back");
	setEntity2DSpeed(bg, -2, 0, 0, 0);
	
	
	Timer* timer2 = addTimer();
	setTimer(timer2, 50, drawBlockages, "blockage-timer");
	Entity2DLayer* lightlayer = addEntity2DLayer(13, "streetlight_beam");
	Entity2DLayer* lightbacklayer = addEntity2DLayer(6, "streetlight_back");
	Entity2D* light = addEntity2D(lightlayer);
	Entity2D* light_back = addEntity2D(lightbacklayer);
	loadEntity2D(light, 0.99, 0.15, "light-beam", "light-beam");
	loadEntity2D(light_back, 0.99, 0.15, "light-back", "light-back");
	setEntity2DSpeed(light, -10, 0, 0, 0);
	setEntity2DSpeed(light_back, -10, 0, 0, 0);
	


	Timer* lightTimer = addTimer();
	setTimer(lightTimer, 100, drawLight, "drawLight");
	Timer* timer = addTimer();
	setTimer(timer, 47, drawRoad, "road");
	Timer* buildTimer = addTimer();
	setTimer(buildTimer, 50, drawBuildings, "drawBuildings");
	Timer* backbuildTimer = addTimer();
	setTimer(backbuildTimer, 35, drawBackBuildings, "drawBackBuildings");
	Timer* bgTimer = addTimer();
	setTimer(bgTimer, 800, drawBackground, "drawbg");
	Timer* propTimer = addTimer();
	int factor = rand() / 30 + 1000;
	setTimer(propTimer, factor, drawProps, "drawProps");
}
extern void updateMainScene()
{
	if (!IS_PAUSED && PLAYER_STATE != FAILED)
	{
		current_score++;
		if (current_score > last_highscore)last_highscore = current_score;
	}
	char* prefix = (char*)calloc(100, sizeof(char));
	strcpy(prefix, "highscore: ");
	strcat(prefix, intToString(last_highscore));
	SDL_Surface* textsurface = TTF_RenderText_Blended(font, prefix, (SDL_Color) { 255, 255, 255, 255 });
	SDL_Texture* textTex = SDL_CreateTextureFromSurface(app.renderer, textsurface);
	SDL_Rect rect = getRectFromTex(textTex, 0, 0);
	rect.x = getRelPosX(0.1);
	rect.y = getRelPosY(0.1);
	SDL_RenderCopy(app.renderer, textTex, NULL, &rect);
	SDL_DestroyTexture(textTex);
	SDL_FreeSurface(textsurface);
	char* prefix2 = (char*)calloc(100, sizeof(char));
	strcpy(prefix2, "current score: ");
	prefix2 = strcat(prefix2, intToString(current_score));
	SDL_Surface* textsurface2 = TTF_RenderText_Blended(font, prefix2, (SDL_Color) { 255, 255, 255, 255 });
	SDL_Texture* textTex2 = SDL_CreateTextureFromSurface(app.renderer, textsurface2);
	SDL_Rect rect2 = getRectFromTex(textTex2, 0, 0);
	rect2.x = getRelPosX(0.1);
	rect2.y = getRelPosY(0.14);
	SDL_RenderCopy(app.renderer, textTex2, NULL, &rect2);
	SDL_DestroyTexture(textTex2);
	SDL_FreeSurface(textsurface2);
	free(prefix);
	free(prefix2);
}
extern void switchToMainScene()
{
	AnimatorLayer* layer = getLayerByTag("Init");
	freeAnimatorLayer(layer);
	Entity2DLayer* layer2 = getLayerByTag("blackback");
	freeEntity2DLayer(layer2);
	layer = getLayerByTag("initbgAnimated");
	freeAnimatorLayer(layer);
	layer2 = getLayerByTag("initbgEntity");
	freeEntity2DLayer(layer2);
	freeButtonPool();
	loadSceneFunc = drawMainScene;
	loadSceneFunc();
	updateSceneFunc = updateMainScene;
#ifdef LOG_ON
	fprintf(fp, "update function switched to main\n");
#endif
}
extern void updatePlayer()
{
	if (PLAYER_STATE == FAILED)return;
	AnimatorLayer* layer = getLayerByTag("Main-Player");
	if (layer == NULL)return;
	Animator* player = getAnimatorByTag(layer, "player");
	if (player == NULL)return;
#ifdef LOG_ON
	fprintf(fp, "player animator caught\n");
#endif
	SDL_Rect rect;
	rect = getRectFromTex(player->entity.tex, player->entity.x, player->entity.y);
	if (player->entity.y + rect.h > getRelPosY(0.95))
	{
		player->entity.y = getRelPosY(0.7);
		player->entity.acceleY = 0;
		player->entity.velY = 0;
		if (PLAYER_STATE == SUSPENDED)
		{
			PLAYER_STATE = NORMAL;
		}
		setAnimator(player, 8, 4, 0.2, 0.7, "player", "player-run");
		player->entity.tex = *player->texList;
	}
	if (player->entity.y <= 0)
	{
		player->entity.velY *= -1;
		if (player->entity.acceleY < 0)player->entity.acceleY = 0;
		player->entity.y = 0;
	}
}
extern void drawInitScene()
{
	AnimatorLayer* logoLayer = addAnimatorLayer(25, "Init");
	Animator* logo = addAnimator(logoLayer);
	setAnimator(logo, 20, 4, 0.55, 0.1, "logo", "logo");
	Button* start_button = addButton();
	loadEntity2D(&start_button->entity, 0.65, 0.65, "start_icon", "start_icon");
	//setEntity2DSpeed(logo, 10, 0, 0, 0);
	Entity2DLayer* backlayer = addEntity2DLayer(24, "blackback");
	Entity2D* back = addEntity2D(backlayer);
	loadEntity2D(back, 0, 0, "blackback", "blackback");
	Entity2D* instruction = addEntity2D(backlayer);
	loadEntity2D(instruction, 0.03, 0.62, "instruct", "instructions");
	AnimatorLayer* initbg = addAnimatorLayer(23, "initbgAnimated");
	Entity2DLayer* initbg2 = addEntity2DLayer(22, "initbgEntity");
	Entity2D* bg = addEntity2D(initbg2);
	loadEntity2D(bg, 0, 0, "initbg", "city_back");
	bg = addEntity2D(initbg2);
	loadEntity2D(bg, 0.1, 0.11, "initbackbuild1", "backbuild_1");
	bg = addEntity2D(initbg2);
	loadEntity2D(bg, 0.3, 0.11, "initbackbuild2", "backbuild_2");
	bg = addEntity2D(initbg2);
	loadEntity2D(bg, 0.55, 0.11, "initbackbuild3", "backbuild_3");
	bg = addEntity2D(initbg2);
	loadEntity2D(bg, 0.75, 0.11, "initbackbuild4", "backbuild_1");
	bg = addEntity2D(initbg2);
	loadEntity2D(bg, 0.95, 0.11, "initbackbuild3", "backbuild_3");
	bg = addEntity2D(initbg2);
	loadEntity2D(bg, 0.05, 0.25, "build-1", "building_1");
	bg = addEntity2D(initbg2);
	loadEntity2D(bg, 0.45, 0.27, "build-2", "building_2");
	bg = addEntity2D(initbg2);
	loadEntity2D(bg, 0.9, 0.23, "build-3", "building_3");
	bg = addEntity2D(initbg2);
	loadEntity2D(bg, 0.3, 0.15, "light_back-1", "light-back");
	bg = addEntity2D(initbg2);
	loadEntity2D(bg, 0.7, 0.15, "light_back-2", "light-back");
	bg = addEntity2D(initbg2);
	loadEntity2D(bg, 0, 0.78, "road-1", "road");
	bg = addEntity2D(initbg2);
	loadEntity2D(bg, 0.2, 0.78, "road-2", "road");
	bg = addEntity2D(initbg2);
	loadEntity2D(bg, 0.4, 0.78, "road-3", "road");
	bg = addEntity2D(initbg2);
	loadEntity2D(bg, 0.6, 0.78, "road-4", "road");
	bg = addEntity2D(initbg2);
	loadEntity2D(bg, 0.8, 0.78, "road-5", "road");
	Animator* initblock = addAnimator(initbg);
	setAnimator(initblock, 15, 2, 0.15, 0.81, "initblock-1", "block_1");
	initblock = addAnimator(initbg);
	setAnimator(initblock, 1, 0, 0.4, 0.55, "init-board-block-fore", "board-block-fore");
	bg = addEntity2D(initbg2);
	loadEntity2D(bg, 0.4, 0.55, "init-board-block-back", "board-block-back");
	initblock = addAnimator(initbg);
	setAnimator(initblock, 15, 2, 0.7, 0.86, "init-block-2", "block_1");
	bg = addEntity2D(initbg2);
	loadEntity2D(bg, 0.3, 0.15, "light_beam-1", "light-beam");
	bg = addEntity2D(initbg2);
	loadEntity2D(bg, 0.7, 0.15, "light_beam-2", "light-beam");
	start_button->func = switchToMainScene;
}
extern void restartGame()
{
	current_score = 0;
	speedFactor = 0;
	currentTick = 0;
	freeAllLayers();
	initTimerManager();
	drawMainScene();
	PLAYER_STATE = NORMAL;
	initButtonPool();
}
extern void updatePlayerFail()
{
	Entity2DLayer* failLayer = getLayerByTag("Fail");
	Entity2D* failScreen = addEntity2D(failLayer);
	loadEntity2D(failScreen, 0, 0.05, "Failscreen", "failscreen");
	Button* restart = addButton();
	loadEntity2D(&restart->entity, 0.4, 0.6, "restartButton", "restart_icon");
	restart->func = restartGame;
	AnimatorLayer* playerlayer = getLayerByTag("Main-Player");
	if (!playerlayer)return;
	Animator* player = getAnimatorByTag(playerlayer, "player");
	if (!player)return;
	SDL_Texture** failtex = getResourceByTag("player-fail");
	changeAnimatorTexture(player, failtex, 2, 25);
	player->entity.y = getRelPosY(0.7);
}
extern void deactivateInvincible()
{
	if (PLAYER_STATE != INVINCIBLE)return;
	PLAYER_STATE = NORMAL;
	Timer* deacTimer = getTimerByTag("deactivateInvincible");
	freeTimer(deacTimer);
}
extern void recoverPlayerStateFromProp()
{
	Timer* recoverTimer = getTimerByTag("recoverPlayerState");
	setTimer(recoverTimer, 100, deactivateInvincible, "deactivateInvincible");
	PLAYER_STATE = INVINCIBLE;
	AnimatorLayer* playerLayer = getLayerByTag("Main-Player");
	if (!playerLayer)return;
	Animator* player = getAnimatorByTag(playerLayer, "player");
	if (!player)return;
	SDL_Texture** texlist = getResourceByTag("player-jump");
	changeAnimatorTexture(player, texlist, 1, 100);
	player->entity.acceleY = 2;
}