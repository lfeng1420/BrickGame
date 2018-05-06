#include "stdafx.h"


const bool BOOM_SHAPE[BOOM_ROW_COUNT][BOOM_COLUMN_COUNT] =
{
	{ true, false, false, true },
	{ false, true, true, false },
	{ false, true, true, false },
	{ true, false, false, true },
};


const int OPPISITE_DIR[DIR_MAX] = 
{
	DIR_LEFT,
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
};


const int BTNID_2_DIR[BTNID_DIRMAX] = 
{
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT,
	DIR_UP,
};


const bool TETRIS_SHAPE[TETRIS_SHAPE_COUNT][SMALL_BRICK_ROW_COUNT][SMALL_BRICK_COLUMN_COUNT] =
{
	{
		{ false, true, true, false },	//0110
		{ true, true, false, false },	//1100
		{ false, false, false, false },
		{ false, false, false, false },
	},
	{
		{ true, false, false, false },	//1000
		{ true, true, false, false },	//1100
		{ false, true, false, false },	//0100
		{ false, false, false, false },
	},

	{
		{ true, true, false, false },	//1100
		{ false, true, true, false },	//0110
		{ false, false, false, false },
		{ false, false, false, false },
	},
	{
		{ false, true, false, false },	//0100
		{ true, true, false, false },	//1100
		{ true, false, false, false },	//1000
		{ false, false, false, false },
	},

	{
		{ false, true, false, false },	//0100
		{ true, true, true, false },	//1110
		{ false, false, false, false },
		{ false, false, false, false },
	},
	{
		{ true, false, false, false },	//1000
		{ true, true, false, false },	//1100
		{ true, false, false, false },	//1000
		{ false, false, false, false },
	},
	{
		{ true, true, true, false },	//1110
		{ false, true, false, false },	//0100
		{ false, false, false, false },
		{ false, false, false, false },
	},
	{
		{ false, true, false, false },	//0100
		{ true, true, false, false },	//1100
		{ false, true, false, false },	//0100
		{ false, false, false, false },
	},

	{
		{ true, true, false, false },	//1100
		{ true, false, false, false },	//1000
		{ true, false, false, false },	//1000
		{ false, false, false, false },
	},
	{
		{ true, true, true, false },	//1110
		{ false, false, true, false },	//0010
		{ false, false, false, false },
		{ false, false, false, false },
	},
	{
		{ false, true, false, false },	//0100
		{ false, true, false, false },	//0100
		{ true, true, false, false },	//1100
		{ false, false, false, false },
	},
	{
		{ true, false, false, false },	//1000
		{ true, true, true, false },	//1110
		{ false, false, false, false },
		{ false, false, false, false },
	},

	{
		{ true, true, false, false },	//1100
		{ false, true, false, false },	//0100
		{ false, true, false, false },	//0100
		{ false, false, false, false },
	},
	{
		{ false, false, true, false },	//0010
		{ true, true, true, false },	//1110
		{ false, false, false, false },
		{ false, false, false, false },
	},
	{
		{ true, false, false, false },	//1000
		{ true, false, false, false },	//1000
		{ true, true, false, false },	//1100
		{ false, false, false, false },
	},
	{
		{ true, true, true, false },	//1110
		{ true, false, false, false },	//1000
		{ false, false, false, false },
		{ false, false, false, false },
	},

	{
		{ true, true, false, false },	//1100
		{ true, true, false, false },	//1100
		{ false, false, false, false },
		{ false, false, false, false },
	},

	{
		{ true, false, false, false },	//1000
		{ true, false, false, false },	//1000
		{ true, false, false, false },	//1000
		{ true, false, false, false },	//1000
	},
	{
		{ true, true, true, true },		//1111
		{ false, false, false, false },
		{ false, false, false, false },
		{ false, false, false, false },
	},
	{
		{ true, false, false, false },	//1000
		{ true, false, false, false },	//1000
		{ true, false, false, false },	//1000
		{ false, false, false, false },
	},
	{
		{ true, false, false, false },	//1000
		{ true, false, false, false },	//1000
		{ false, false, false, false },
		{ false, false, false, false },
	},
	{
		{ true, false, false, true },	//1001
		{ false, true, true, false },	//0110
		{ false, true, true, false },	//0110
		{ true, false, false, true },	//1001
	},
	{
		{ true, false, false, false },	//1000
		{ false, false, false, false },	//0000
		{ false, false, false, false },
		{ false, false, false, false },
	},
};


const bool DEFAULT_SHAPE[ROW_COUNT * COLUMN_COUNT] =
{
	false, false, false, false, false, false, false, false, false, false, false, false, false, false,
	false, false, true, true, true, true, true, false, false, false, false, false, false, false,
	false, false, false, true, false, false, false, true, false, false, false, false, false, false,
	false, false, false, true, true, true, true, false, false, false, false, false, false, false,
	false, false, false, true, false, false, false, true, false, false, false, false, false, false,
	false, false, true, true, true, true, true, false, false, false, false, false, false, false,
	false, false, false, false, false, false, false, false, false, false, false, false, false, false,
	false, false, false, true, true, false, false, true, true, true, true, true, false, false,
	false, false, false, false, true, false, false, false, false, false, false, true, false, false,
	false, false, false, false, true, false, false, true, true, true, true, true, false, false,
	false, false, false, false, true, false, false, true, false, false, false, false, false, false,
	false, false, false, false, true, false, false, true, false, false, false, false, false, false,
	false, false, false, true, true, true, false, true, true, true, true, true, false, false,
	false, false, false, false, false, false, false, false, false, false, false, false, false, false,
	false, false, false, false, false, false, false, true, true, true, true, false, false, false,
	false, false, false, false, false, false, true, false, false, false, false, false, false, false,
	false, false, false, false, false, false, true, false, false, false, true, true, false, false,
	false, false, false, false, false, false, true, false, false, false, false, true, false, false,
	false, false, false, false, false, false, false, true, true, true, true, false, false, false,
	false, false, false, false, false, false, false, false, false, false, false, false, false, false,
};


const int TETRIS_SHAPE_ROW_COUNT[TETRIS_SHAPE_COUNT] = { 2, 3, 2, 3, 2, 3, 2, 3, 3, 2, 3, 2, 3, 2, 3, 2, 2, 4, 1, 3, 2, 4, 1 };

const int TETRIS_SHAPE_COL_COUNT[TETRIS_SHAPE_COUNT] = { 3, 2, 3, 2, 3, 2, 3, 2, 2, 3, 2, 3, 2, 3, 2, 3, 2, 1, 4, 1, 1, 4, 1 };

const int TETRIS_TYPE_START_IDX[TETRIS_SHAPE_COUNT] = { 0, 2, 4, 8, 12, 16, 17, 19, 20, 21, 22 };

// Sound string
#if CC_TARGET_PLATFORM == CC_PLATFORM_WP8 
const char* BGM_START = "Sounds/bgm.wav";
const char* EFFECT_ADD = "Sounds/add.wav";
const char* EFFECT_BOOM = "Sounds/boom.wav";
const char* EFFECT_CHANGE = "Sounds/change.wav";
const char* EFFECT_DELETE = "Sounds/delete.wav";
const char* EFFECT_NEXT = "Sounds/next.wav";
const char* EFFECT_START = "Sounds/start.wav";
const char* EFFECT_SOUNDOFF = "Sounds/sndoff.wav";
const char* EFFECT_SOUNDON = "Sounds/sndon.wav";
const char* EFFECT_WALL = "Sounds/wall.wav";

const char* FONT_NAME = "DengXian";
#else
const char* BGM_START = "Sounds/bgm.mp3";
const char* EFFECT_ADD = "Sounds/add.mp3";
const char* EFFECT_BOOM = "Sounds/boom.mp3";
const char* EFFECT_CHANGE = "Sounds/change.mp3";
const char* EFFECT_DELETE = "Sounds/delete.mp3";
const char* EFFECT_NEXT = "Sounds/next.mp3";
const char* EFFECT_START = "Sounds/start.mp3";
const char* EFFECT_SOUNDOFF = "Sounds/sndoff.mp3";
const char* EFFECT_SOUNDON = "Sounds/sndon.mp3";
const char* EFFECT_WALL = "Sounds/wall.mp3";

const char* FONT_NAME = "Arial";
#endif


// Setup string
const char* BTN_SPR = "blank.png";
const char* BTN_CLICK_SPR = "sliderprogress.png";