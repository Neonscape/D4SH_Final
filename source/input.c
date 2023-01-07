#include"common.h"
#include"defs.h"
#include"structs.h"
#include"mouse_widgets.h"
#include"main.h"
#include"init.h"
extern int PLAYER_STATE = NORMAL;
extern void doInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		//fprintf(fp, "event polled, type = %d\n", event.type);
		switch (event.type)
		{
		case SDL_QUIT:
			fprintf(scoref, "%d", last_highscore);
			fclose(scoref);
			fclose(fp);
			exit(0);
		case SDL_MOUSEBUTTONDOWN:
			checkButtonPool(event.button);
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_w:
				if (event.key.repeat)continue;
				if ((PLAYER_STATE == NORMAL || PLAYER_STATE == INVINCIBLE) && !IS_PAUSED)
				{
					setPlayerEntity2DSpeed(0, -18, 0, 1.3);
					SDL_Texture** texlist = getResourceByTag("player-jump");
					AnimatorLayer* playerlayer = getLayerByTag("Main-Player");
					if (playerlayer == NULL)break;
					Animator* player = getAnimatorByTag(playerlayer, "player");
					if (player == NULL)break;
					changeAnimatorTexture(player, texlist, 1, 1);
					//fprintf(fp, "Key W Pressed\n");

					PLAYER_STATE = SUSPENDED;
				}
				break;
			case SDLK_p:
				if (PLAYER_STATE == FAILED)break;
				if (!IS_PAUSED)
				{
					Mix_PauseMusic();
				}
				else
				{
					Mix_ResumeMusic();
				}
				IS_PAUSED ^= 1;
				break;
			case SDLK_s:
				if (event.key.repeat)break;
				if ((PLAYER_STATE == NORMAL || PLAYER_STATE == INVINCIBLE) && !IS_PAUSED)
				{
					//fprintf(fp, "Key S pressed\n");
					SDL_Texture** texList = getResourceByTag("player-duck");
					AnimatorLayer* playerlayer = getLayerByTag("Main-Player");
					if (!playerlayer)return;
					Animator* player = getAnimatorByTag(playerlayer, "player");
					if (!player)return;
					changeAnimatorTexture(player, texList, 1, 1);
					player->entity.collh = 80;
					player->entity.collw = 140;
					player->entity.y += 100;
					PLAYER_STATE = DUCKING;
				}
				break;
			case SDLK_ESCAPE:
				fprintf(scoref, "%d", last_highscore);
				fclose(scoref);
				fclose(fp);
				exit(0);
			default:
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_s:
				if (PLAYER_STATE == DUCKING)
				{
					//fprintf(fp, "Key S released\n");
					AnimatorLayer* layer = getLayerByTag("Main-Player");
					if (!layer)return;
					Animator* player = getAnimatorByTag(layer, "player");
					if (!player)return;
					setAnimator(player, 8, 4, 0.2, 0.7, "player", "player-run");
					setEntity2DCollideBox(&player->entity, 70, 170, 15, 0);
					//fprintf(fp, "changing player state to NORMAL");
					PLAYER_STATE = NORMAL;
				}
			}
			break;
		default:
			break;
		}

	}
}