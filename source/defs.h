#pragma once
#define SCREEN_HEIGHT 900
#define SCREEN_WIDTH 1600
#define MEMORY_ALLOC 1000
#define FRAME_RATE 75
#define DELTA_TIME 1000 / FRAME_RATE
#define WITHIN(l, r, val) (val <= r) && (val >= l)
#define MAX(a, b) a > b ? a : b
#define MIN(a, b) a < b ? a : b
#define REL_RAND_THRESHOLD 0.95
#define RAND_THRESHOLD (int)(REL_RAND_THRESHOLD * RAND_MAX)
#define SPEED_RATIO (0.00225258 / (double)FRAME_RATE)		//that float number was calculated by myself
enum PLAYER_STATE_CODE
{
	NORMAL = 0,
	SUSPENDED,
	FAILURE,
	DUCKING,
	FAILED,
	READY,
	FLYING,
	INVINCIBLE
};