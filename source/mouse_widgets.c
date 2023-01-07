#pragma once
#include"mouse_widgets.h"
#include"main.h"
ButtonLayer buttonpool;
extern void initButton(Button* button)
{
	initEntity2D(&button->entity);
	button->func = NULL;
}
extern void initButtonPool()
{
	memset(&buttonpool, 0, sizeof(buttonpool));
	buttonpool.layer = (Button*)calloc(MEMORY_ALLOC, sizeof(Button));
}
extern Button* addButton()
{
	if (buttonpool.cnt2)
	{
		initButton((buttonpool.layer + buttonpool.freeptr[buttonpool.cnt2 - 1]));
		Button* pt = (buttonpool.layer + buttonpool.freeptr[buttonpool.cnt2 - 1]);
		pt->entity.id = buttonpool.freeptr[buttonpool.cnt2 - 1];
		buttonpool.cnt2--;
		return pt;
	}
	buttonpool.cnt++;
	initButton((buttonpool.layer + buttonpool.cnt - 1));
	(buttonpool.layer + buttonpool.cnt - 1)->entity.id = buttonpool.cnt - 1;
	return (Button*)(buttonpool.layer + buttonpool.cnt - 1);
}
extern void delButton(Button* button)
{
	int id = button->entity.id;
	Button* pt = buttonpool.layer + id;
	buttonpool.freeptr[buttonpool.cnt2++] = pt->entity.id;
	//initButton(button);
	pt->entity.id = -1;
}
extern void checkButton(Button button, SDL_MouseButtonEvent _event)
{
	if (_event.button != SDL_BUTTON_LEFT)return;
	int xl = button.entity.x, yl = button.entity.y, dx, dy;
	SDL_QueryTexture(button.entity.tex, NULL, NULL, &dx, &dy);
	if (WITHIN(xl, xl + dx, _event.x) && WITHIN(yl, yl + dy, _event.y))
	{
		button.func();
#ifdef LOG_ON
		fprintf(fp, "button %s pressed\n", button.entity.tag);
#endif
	}
	return;
}
extern void updateButtonPool()
{
	for (int i = 0; i < buttonpool.cnt; i++)
	{
		if ((buttonpool.layer + i)->entity.id == -1)continue;
		drawEntity2D(&(buttonpool.layer + i)->entity);
	}
}
extern void checkButtonPool(SDL_MouseButtonEvent _event)
{
	for (int i = 0; i <= buttonpool.cnt; i++)
	{
		if ((buttonpool.layer + i)->entity.id == -1)continue;
		checkButton(*(buttonpool.layer + i), _event);
	}
}
extern void freeButtonPool()
{
	free(buttonpool.layer);
	initButtonPool();
}