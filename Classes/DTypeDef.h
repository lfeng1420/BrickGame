#pragma once

// POSITION
struct POSITION
{
    int x;
    int y;
};


struct TPassAddScoreData
{
	int nInterval;
	int nCount;
};


struct TBoomAnimData
{
	int nCurFrameCount;
	int nInterval;
	POSITION stPos;
	bool bVisibleFlag;
};


struct TGameSceneContext
{
	bool arrBrickState[ROW_COUNT * COLUMN_COUNT];	// Bricks state
	bool arrSmallBrickState[SMALL_BRICK_COUNT];		// Small bricks state
	SEventContextDataUpdate	stGameData;				// Current run game data
	bool bPauseFlag;								// Game pause flag
	unsigned int uiStartBtnIdx;						// Start button select index
	bool bPlayingBGMFlag;							// Playing BGM Flag
};