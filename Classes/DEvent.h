#pragma once

enum EnEventID
{
    // UI Event
    EVENT_GAME_START,           // Start game
    EVENT_BUTTON,               // Button press or release
	EVENT_FRAME_UPDATE,			// Update frame
	EVENT_PLAY_EFFECT,			// Play effect
	EVENT_PLAY_BGM,				// Control background music
	EVENT_VIBRATE,				// Vibrate

    // Logic Event
    EVENT_BRICKSTATE_UPDATE,    // some bricks state has changed
    EVENT_DATA_UPDATE,          // Data update
	EVENT_GAME_OVER,			// Game over
	EVENT_SAVE_DATA,			// Save data

    EVENT_MAXID,                // max, invalid value
};


//////// Define event context. ////////
// EVENT_GAMEID_START
struct SEventContextGameStart
{
    int nGameID;  // Game id
    int nLevel;
    int nSpeed;
	int nScore;
	int nLife;		// Life count, -1 is infinite
};

// EVENT_BUTTON
struct SEventContextButton
{
    int nButtonID;  // Button id
    bool bPressedFlag;
};

// EVENT_FRAME_UPDATE
struct SEventContextFrameUpdate
{
	float dt;
};

// EVENT_SOUND
struct SEventPlayEffect
{
	const char* szSound;
};

// EVENT_BGMUSIC
struct SEventPlayBGM
{
	bool bPlayFlag;
	bool bLoopFlag;
};

// EVENT_VIBRATE
struct SEventVibrate
{
	bool bShortFlag;
};

// EVENT_BRICKSTATE_UPDATE
typedef map<int, bool>	TMap_BrickState;
struct SEventContextBrickStateUpdate
{
	TMap_BrickState mapUpdateBricks;
    bool bSmallBrickFlag;
	bool bForceUpdateFlag;
};

// EVENT_DATA_UPDATE
struct SEventContextDataUpdate
{
    int nSpeed;
    int nLevel;
    int nCurScore;
	int nMaxScore;
};

// EVENT_GAME_OVER
struct SEventContextGameOver
{
};

// EVENT_SAVE_DATA
struct SEventContextSaveData
{
	bool bForceFlag;
};