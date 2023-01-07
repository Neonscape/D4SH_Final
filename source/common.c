#include"common.h"
#include"defs.h"
#include"structs.h"
#include"init.h"
#include"main.h"
#include"input.h"
#include"draw.h"
LayerManager manager;
TimerManager tmanager;
ResourceManager rmanager;
extern int total_Entities = 0;
extern int IS_PAUSED = 0;
extern int last_highscore = 0 , current_score = 0;
extern double speedFactor = 0;
extern int currentTick = 0;
extern int getIntLength(int x)
{
	int ans = 0;
	while (x)
	{
		ans++;
		x /= 10;
	}
	return ans;
}
extern char* intToString(int x)
{
	if (!x)return "0";
	int p = getIntLength(x);
	char* temp = (char*)calloc(p, sizeof(char));
	for (int i = 0; i < p; i++)
	{
		*(temp + i) = '0';
	}
	for (int i = p - 1; x;i--)
	{
		*(temp + i) += x % 10;
		x /= 10;
	}
	*(temp + p) = 0;
	return temp;
}
extern int getRelPosX(double x)//x for left - right, from 0 to 1
{
	return round(x * SCREEN_WIDTH);
}
extern int getRelPosY(double y)//y for up and down, from 0 to 1
{
	return round(y * SCREEN_HEIGHT);
}
extern void loadEntity2D(Entity2D* entity, double relX, double relY, const char* tag, const char* restag)
{
	SDL_Texture** texList = getResourceByTag(restag);
	if (!texList)
	{
#ifdef LOG_ON
		fprintf(fp, "error loading image %s\n", restag);
#endif
		exit(1);
	}
	entity->tex = *texList;
	entity->x = getRelPosX(relX);
	entity->y = getRelPosY(relY);
	entity->tag = tag;
}
extern SDL_Rect getRectFromTex(SDL_Texture* tex, int x, int y)
{
	SDL_Rect rect;
	SDL_QueryTexture(tex, NULL, NULL, &rect.w, &rect.h);
	return rect;
}
extern SDL_Rect getRectFromEntity2D(Entity2D* entity)
{
	SDL_Rect rect;
	rect.x = (int)entity->x;
	rect.y = (int)entity->y;
	SDL_QueryTexture(entity->tex, NULL, NULL, &rect.w, &rect.h);
	return rect;
}
extern void updateEntity2D(Entity2D* entity)
{
	if (!IS_PAUSED)
	{
		if (PLAYER_STATE == FAILED)
		{
			entity->velX = 0;
			entity->velY = 0;
			entity->acceleX = 0;
			entity->acceleY = 0;
			return;
		}
		entity->x += entity->velX;
		entity->y += entity->velY;
		entity->velX += entity->acceleX;
		entity->velY += entity->acceleY;
	}
}
extern void drawEntity2D(Entity2D* entity)
{
	SDL_Rect rect = getRectFromEntity2D(entity);
	SDL_RenderCopy(app.renderer, entity->tex, NULL, &rect);
}
extern void initEntity2DLayer(Entity2DLayer* layer)
{
	memset(layer, 0, sizeof(Entity2DLayer));
	layer->layer = (Entity2D*)calloc(MEMORY_ALLOC, sizeof(Entity2D));
}
extern void initEntity2D(Entity2D* entity)
{
	entity->acceleX = 0;
	entity->acceleY = 0;
	entity->id = 0;
	entity->tex = NULL;
	entity->velX = 0;
	entity->velY = 0;
	entity->x = 0;
	entity->y = 0;
}
extern Entity2D* addEntity2D(Entity2DLayer* layer)
{
	if (layer->cnt2)
	{
		Entity2D* temp = (layer->layer + layer->freeptr[layer->cnt2]);
		initEntity2D(temp);
		temp->id = layer->freeptr[layer->cnt2];
		layer->cnt2--;
		return temp;
	}
	Entity2D* pt = layer->layer;
	pt += layer->cnt;
	pt->id = layer->cnt;
	layer->cnt++;
	return pt;
}
extern void freeEntity2DLayer(Entity2DLayer* layer)
{
	
	for (int i = 0; i < layer->cnt; i++)
	{
		Entity2D* pt = layer->layer + i;
		if (!pt || pt->id == -1)continue;
		freeEntity2D(pt);
	}
#ifdef LOG_ON
	fprintf(fp, "Entity2Dlayer %s freed\n", layer->tag);
#endif
	//free(layer->layer);
	layer->cnt = 0;
	layer->layer = NULL;
}
extern void setEntity2DSpeed(Entity2D* entity, double velx, double vely, double accx, double accy)
{
	entity->velX = velx;
	entity->velY = vely;
	entity->acceleX = accx;
	entity->acceleY = accy;
}
extern int getEntityWidth(Entity2D* entity)
{
	int ans, p;
	SDL_QueryTexture(entity->tex, NULL, NULL, &ans, &p);
	return ans;
}
extern int getEntityHeight(Entity2D* entity)
{
	int ans, p;
	SDL_QueryTexture(entity->tex, NULL, NULL, &ans, &p);
	return p;
}
extern int isVisible(Entity2D* entity)
{
#ifdef LOG_ON
	fprintf(fp, "entity %s-%d axis: x = %lf, y = %lf\n", entity->tag, entity->id, entity->x, entity->y);
#endif
	return WITHIN(-getEntityWidth(entity), SCREEN_WIDTH, entity->x) && WITHIN(-getEntityHeight(entity), SCREEN_HEIGHT, entity->y);
}
extern void freeEntity2D(Entity2D* entity)
{
	int id = entity->id;
#ifdef LOG_ON
	if (strlen(entity->tag))
	{
		fprintf(fp, "entity %s-%d freed\n", entity->tag, entity->id);
	}
#endif
	initEntity2D(entity);
	entity->id = -1;
}
extern void updateEntity2DLayer(Entity2DLayer* layer)
{
#ifdef LOG_ON
	fprintf(fp, "%s\n", layer->tag);
#endif
	for (int i = 0; i < layer->cnt; i++)
	{
		if ((layer->layer + i)->id == -1)continue;
		Entity2D *temp = (layer->layer + i);
		if (!isVisible(temp))
		{
			freeEntity2D((layer->layer + i));
			layer->freeptr[++layer->cnt2] = i;
			continue;
		}
		updateEntity2D(temp);
		drawEntity2D(temp);
	}
#ifdef LOG_ON
	fprintf(fp, "EntityLayer %s updated\n", layer->tag);
#endif

}
extern void initAnimator(Animator* animator)
{
	initEntity2D(&animator->entity);
	animator->currentFrame = 0;
	animator->framecount = 0;
	animator->framerate = 0;
	animator->texList = NULL;
	animator->totalFrames = 0;
}
extern void initAnimatorLayer(AnimatorLayer* layer)
{
	memset(layer, 0, sizeof(AnimatorLayer));
	layer->layer = (Animator*)calloc(MEMORY_ALLOC, sizeof(Animator));
}
extern Animator* addAnimator(AnimatorLayer* layer)
{
	if (layer->cnt2)
	{
		Animator* pt = (layer->layer + layer->freeptr[layer->cnt2]);
		layer->cnt2--;
		initAnimator(pt);
		return pt;
	}
	Animator* pt = (layer->layer + layer->cnt);
	initAnimator(pt);
	pt->entity.id = layer->cnt++;
	return pt;
}
extern void freeAnimator(Animator* animator)
{
	int id = animator->entity.id;
#ifdef LOG_ON
	fprintf(fp, "Animator %s freed\n", animator->tag);
#endif
	initAnimator(animator);
	animator->entity.id = -1;
	animator->texList = NULL;
}
extern void updateAnimator(Animator* animator)
{
	if (!IS_PAUSED)
	{
		if (animator->totalFrames > 1)
		{
			animator->framecount++;
			if (animator->framecount == animator->framerate)
			{
				animator->currentFrame++;
				if (animator->currentFrame == animator->totalFrames)
				{
					animator->currentFrame = 0;
				}
				animator->framecount = 0;
				animator->entity.tex = *(animator->texList + animator->currentFrame);
			}
		}
		updateEntity2D(&animator->entity);
#ifdef LOG_ON
		fprintf(fp, "Animator %s updated\n", animator->tag);
		if (!strcmp(animator->tag, "player"))
		{
			fprintf(fp, "player animator updated, current y speed : %lf, current y acce: %lf\n", animator->entity.velY, animator->entity.acceleY);
		}
#endif
	}
}
extern void freeAnimatorLayer(AnimatorLayer* layer)
{
	for (int i = 0; i < layer->cnt; i++)
	{
		Animator* pt = (layer->layer + i);
		if (!pt || pt->entity.id == -1)continue;
		freeAnimator(pt);
	}
	//free(layer->layer);
	initAnimatorLayer(layer);
}
extern void setAnimator(Animator* animator, int totalFrames, int framerate, double relx, double rely, const char* tag, const char* restag)
{
	SDL_Texture** texlist = getResourceByTag(restag);
	if (!texlist)
	{
		fprintf(fp, "error loading image %s\n", restag);
		exit(1);
	}
	animator->texList = texlist;
	animator->currentFrame = 0;
	animator->entity.tag = tag;
	animator->entity.tex = *texlist;
	animator->framecount = 0;
	animator->framerate = framerate;
	animator->entity.x = getRelPosX(relx);
	animator->entity.y = getRelPosY(rely);
	animator->totalFrames = totalFrames;
	animator->tag = tag;
}
extern void updateAnimatorLayer(AnimatorLayer* layer)
{
#ifdef LOG_ON
	fprintf(fp, "updating AnimatorLayer %s\n", layer->tag);
	fprintf(fp, "%s\n", layer->tag);
#endif
	for (int i = 0; i < layer->cnt; i++)
	{
		if ((layer->layer + i)->entity.id == -1)continue;
		if (!isVisible(&(layer->layer + i)->entity))
		{
#ifdef LOG_ON
			fprintf(fp, "Animator %s-%d is not visible, clearing\n", (layer->layer + i)->tag, (layer->layer + i)->entity.id);
#endif
			freeAnimator(layer->layer + i);
			layer->freeptr[++layer->cnt2] = i;
			continue;
		}
		updateAnimator((layer->layer + i));
		drawEntity2D(&(layer->layer + i)->entity);
	}
#ifdef LOG_ON
	fprintf(fp, "AnimatorLayer %s updated, total Animators %d\n", layer->tag, layer->cnt - layer->cnt2);
#endif
}
extern void initLayerManager()
{
	manager.layerPtr = (void**)calloc(MEMORY_ALLOC, sizeof(void*));
	manager.layerCount = 0;
	manager.type = (char*)calloc(MEMORY_ALLOC, sizeof(char));
	for (int i = 0; i < 50; i++)
	{
		*(manager.layerPtr + i) = NULL;
	}
}
extern Entity2DLayer* addEntity2DLayer(int index, const char* tag)
{
	if (*(manager.layerPtr + index) != NULL)return NULL;
	*(manager.layerPtr + index) = (Entity2DLayer*)calloc(1, sizeof(Entity2DLayer));
	Entity2DLayer* layerPt = *(manager.layerPtr + index);
	initEntity2DLayer(layerPt);
	layerPt->tag = tag;
	layerPt->layerid = index;
	layerPt->type = "Entity2D";
	manager.layerCount = MAX(manager.layerCount, index);
	*(manager.type + index) = 'E';
	return layerPt;
}
extern AnimatorLayer* addAnimatorLayer(int index, const char* tag)
{
	if (*(manager.layerPtr + index) != NULL)return NULL;
	*(manager.layerPtr + index) = (AnimatorLayer*)calloc(1, sizeof(AnimatorLayer));
	AnimatorLayer* layerPt = *(manager.layerPtr + index);
	initAnimatorLayer(layerPt);
	layerPt->layerid = index;
	layerPt->tag = tag;
	layerPt->type = "Animator";
	manager.layerCount = MAX(manager.layerCount, index);
	*(manager.type + index) = 'A';
	return layerPt;
}
extern void* getLayerByIndex(int index)
{
	return *(manager.layerPtr + index);
}
extern void* getLayerByTag(const char* tag)
{
	for (int i = 0; i <= manager.layerCount; i++)
	{
		if (*(manager.layerPtr + i) == NULL)continue;
		if (*(manager.type + i) == 'E')
		{
			Entity2DLayer* layerpt = (Entity2DLayer*)*(manager.layerPtr + i);
			if (!strcmp(layerpt->tag, tag))return layerpt;
		}
		else
		{
			AnimatorLayer* layerpt = (AnimatorLayer*)*(manager.layerPtr + i);
			if (!strcmp(layerpt->tag, tag))return layerpt;
		}
	}
	return NULL;
}
extern void updateLayers()
{
	for (int i = 0; i <= manager.layerCount; i++)
	{
		if (*(manager.layerPtr + i) == NULL)continue;
#ifdef LOG_ON
		fprintf(fp, "layer %d is ", i);
#endif
		if (*(manager.type + i) == 'E')
		{
			updateEntity2DLayer(*(manager.layerPtr + i));
		}
		else
		{
			updateAnimatorLayer(*(manager.layerPtr + i));
		}
	}
}
extern void removeLayer(int index)
{
	if (*(manager.type + index) == 'E')
	{
		Entity2DLayer* layer = *(manager.layerPtr + index);
		free(layer->layer);
		free(layer);
		*(manager.layerPtr + index) = NULL;
	}
	else
	{
		AnimatorLayer* layer = *(manager.layerPtr + index);
		free(layer->layer);
		free(layer);
		*(manager.layerPtr + index) = NULL;
	}
	*(manager.type + index) = 0;
}
extern void initTimer(Timer* timer)
{
	timer->currCount = 0;
	timer->func = NULL;
	timer->id = 0;
	timer->loopCount = 0;
	timer->tag = "";
}
extern void initTimerManager()
{
	memset(&tmanager, 0, sizeof(TimerManager));
	tmanager.timerList = (Timer*)calloc(MEMORY_ALLOC, sizeof(Timer));
}
extern void setTimer(Timer* timer, int loopCount, void (*func)(void), const char* tag)
{
	timer->loopCount = loopCount;
	timer->func = func;
	timer->tag = tag;
}
extern void freeTimer(Timer* timer)
{
	tmanager.freeptr[++tmanager.cnt2] = timer->id;
	timer->id = -1;
}
extern void updateTimer(Timer* timer)
{
	if (timer->id == -1)return;
	if (!IS_PAUSED)
	{
		timer->currCount++;
		if (timer->currCount == timer->loopCount)
		{
			timer->func();
			timer->currCount = 0;
		}
	}
}
extern Timer* addTimer()
{
	if (tmanager.cnt2)
	{
		Timer* pt = (tmanager.timerList + tmanager.freeptr[tmanager.cnt2--]);
		initTimer(pt);
		pt->id = tmanager.freeptr[tmanager.cnt2 + 1];
		return pt;
	}
	Timer* pt = (tmanager.timerList + tmanager.cnt);
	initTimer(pt);
	pt->id = tmanager.cnt;
	tmanager.cnt++;
	return pt;
}
extern void updateTimerManager()
{
#ifdef LOG_ON
	fprintf(fp, "updating %d timers\n", tmanager.cnt);
#endif
	for (int i = 0; i < tmanager.cnt; i++)
	{
		if ((tmanager.timerList + i)->id == -1)continue;
#ifdef LOG_ON
		fprintf(fp, "updating timer %s\n", (tmanager.timerList + i)->tag);
		clock_t start_t = clock();
#endif

		updateTimer(tmanager.timerList + i);
#ifdef LOG_ON
		clock_t end_t = clock();
		int deltatime_ms = end_t - start_t;
		fprintf(fp, "update timer %s took %d ms\n", (tmanager.timerList + i)->tag, deltatime_ms);
#endif
	}
}
extern Timer* getTimerByTag(const char* tag)
{
	for (int i = 0; i < tmanager.cnt; i++)
	{
		if ((tmanager.timerList + i)->id == -1)continue;
		if (!strcmp(tag, (tmanager.timerList + i)->tag))return (tmanager.timerList + i);
	}
	return NULL;
}
extern Animator* getAnimatorByTag(AnimatorLayer* layer, const char* tag)
{
	for (int i = 0; i < layer->cnt; i++)
	{
		if ((layer->layer + i)->entity.id == -1)continue;
		if (!strcmp((layer->layer + i)->tag, tag))return (layer->layer + i);
	}
	return NULL;
}
extern void updateTotalEntities()
{
	total_Entities = 0;
	for (int i = 0; i <= manager.layerCount; i++)
	{
		if (*(manager.layerPtr + i) == NULL)continue;
		if (*(manager.type + i) == 'E')
		{
			Entity2DLayer* pt = *(manager.layerPtr + i);
			total_Entities += (pt->cnt - pt->cnt2);
		}
		else
		{
			AnimatorLayer* pt = *(manager.layerPtr + i);
			total_Entities += (pt->cnt - pt->cnt2);
		}
	}
}
extern void setPlayerEntity2DSpeed(double velx, double vely, double accex, double accey)
{
	AnimatorLayer* layer = getLayerByTag("Main-Player");
	if (layer == NULL)return;
	Animator* player = getAnimatorByTag(layer, "player");
	if (player == NULL)return;
	setEntity2DSpeed(&player->entity, velx, vely, accex, accey);
}
extern void setEntity2DCollideBox(Entity2D* entity, int w, int h, int disx, int disy)
{
	if (entity == NULL)return;
	entity->collw = w, entity->collh = h;
	entity->disx = disx, entity->disy = disy;
	return;
}
extern int isCollided(Entity2D* entity1, Entity2D* entity2)
{
	if (entity1->y + entity1->collh + entity1->disy < entity2->y + entity2->disy)return 0;
	if (entity1->y + entity1->disy > entity2->y + entity2->collh + entity2->disy)return 0;
	if (entity1->x + entity1->collw + entity1->disx < entity2->x + entity2->disx)return 0;
	if (entity1->x + entity1->disx > entity2->x + entity2->collw + entity2->disx)return 0;
	return 1;
}
extern void updateCollision(const char* tag)
{
#ifdef LOG_ON
	fprintf(fp, "player state before updating collision is %d\n", PLAYER_STATE);
#endif
	if (PLAYER_STATE == FAILED)return;
	if (!IS_PAUSED)
	{
		AnimatorLayer* layer = getLayerByTag(tag);
		if (layer == NULL)return;
		AnimatorLayer* playerlayer = getLayerByTag("Main-Player");
		if (playerlayer == NULL)return;
		Animator* player = getAnimatorByTag(playerlayer, "player");
		if (player == NULL)return;
		for (int i = 0; i < layer->cnt; i++)
		{
			if ((layer->layer + i)->entity.id == -1)continue;
			if (!((layer->layer + i)->entity.collh) || !((layer->layer + i)->entity.collw))continue;
			int ans = isCollided(&player->entity, &(layer->layer + i)->entity);
#ifdef LOG_ON
			fprintf(fp, "Checking collision for %s and %s - %d\n", player->tag, (layer->layer + i)->tag, (layer->layer + i)->entity.id);
#endif
			if (ans)
			{
				if (!strcmp(layer->tag, "Blockages") || !strcmp(layer->tag, "Board-Blockage-Fore"))
				{
					if (ans && PLAYER_STATE != FAILED)
					{
#ifdef LOG_ON
						fprintf(scoref, "%d", last_highscore);
						fprintf(fp, "changed player state to 'failed'\n");
#endif
						PLAYER_STATE = FAILED;
						updatePlayerFail();
						return;
					}
				}
				if (!strcmp(layer->tag, "props"))
				{
					SDL_Texture** texlist = getResourceByTag("player-swing");
					changeAnimatorTexture(player, texlist, 4, 10);
					Timer* recoverPlayerStateTimer = addTimer();
					setTimer(recoverPlayerStateTimer, 800, recoverPlayerStateFromProp, "recoverPlayerState");
					PLAYER_STATE = FLYING;
					player->entity.y = getRelPosY(0.4);
					player->entity.acceleY = 0;
					player->entity.velY = 0;
					current_score += 900;
				}
			}
		}
	}
}
extern void changeAnimatorTexture(Animator* animator, SDL_Texture** texlist, int totalFrames, int framerate)
{
	animator->texList = texlist;
	animator->totalFrames = totalFrames;
	animator->framerate = framerate;
	animator->currentFrame = 0;
	animator->framecount = 0;
	animator->entity.tex = *(texlist);
}
extern void initResourceManager()
{
	rmanager.resList = (Resource*)calloc(MEMORY_ALLOC, sizeof(Resource));
	rmanager.totalRes = 0;
}
extern void loadResource(const char* path, int type/*0 for single image, 1 for animations*/, int totalTex, const char* tag)
{
	(rmanager.resList + rmanager.totalRes)->tag = tag;
	if (!type)//one image
	{
		(rmanager.resList + rmanager.totalRes)->texCount = 1;
		(rmanager.resList + rmanager.totalRes)->texList = (SDL_Texture**)calloc(1, sizeof(SDL_Texture*));
		*((rmanager.resList + rmanager.totalRes)->texList) = IMG_LoadTexture(app.renderer, path);
	}
	else
	{
		(rmanager.resList + rmanager.totalRes)->texCount = totalTex;
		(rmanager.resList + rmanager.totalRes)->texList = (SDL_Texture**)calloc(totalTex, sizeof(SDL_Texture*));
		char path_suffix[10] = ".png";
		char* index2 = intToString(totalTex);
		char* path_total = (char*)calloc(strlen(path) + strlen(index2) + strlen(path_suffix) + 1, sizeof(char));
		for (int i = 0; i < totalTex; i++)
		{
			memset(path_total, 0, sizeof(path_total));
			char* index = intToString(i);
			strcpy(path_total, path);
			strcpy(path_total + strlen(path), index);
			strcpy(path_total + strlen(path) + strlen(index), path_suffix);
			*(path_total + strlen(path) + strlen(index) + strlen(path_suffix)) = 0;
			*((rmanager.resList + rmanager.totalRes)->texList + i) = IMG_LoadTexture(app.renderer, path_total);
		}
	}
	rmanager.totalRes++;
}
extern SDL_Texture** getResourceByTag(const char* tag)
{
	for (int i = 0; i < rmanager.totalRes; i++)
	{
		if (!strcmp((rmanager.resList + i)->tag, tag))
		{
			return (rmanager.resList + i)->texList;
		}
	}
	return NULL;
}
extern void freeAllLayers()
{
	for (int i = 0; i <= manager.layerCount; i++)
	{
		if (*(manager.layerPtr + i) == NULL)continue;
		if (*(manager.type + i) == 'E')
		{
			freeEntity2DLayer(*(manager.layerPtr + i));
		}
		else
		{
			freeAnimatorLayer(*(manager.layerPtr + i));
		}
	}
	free(manager.layerPtr);
	manager.layerCount = 0;
	memset(manager.type, 0, sizeof(manager.type));
	initLayerManager();
}
extern void updateSpeedFactor()
{
	speedFactor = exp((double)SPEED_RATIO * (double)currentTick);
}