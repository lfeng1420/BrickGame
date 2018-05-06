#pragma once
#include "ITimerHandler.h"

class CDataManager : public ITimerHandler
{
public:
    // Create
    bool Create();

    // Get game anim data
    // the array length must equal with (ROW_COUNT * COLUMN_COUNT)
    bool GetGameAnimData(int nGameID, bool* arrAnimData, int nFrameIdx);

	// Save high score data
	bool SaveHighScore();

	// Get high score
	int GetHighScore(int nGameID);

	// Set high score
	void SetHighScore(int nGameID, int nHighScore);

	// Get string
	const char* GetString(int nLangID, int nStrID);

	// Save tetris data
	bool SaveTetrisData(const char* szPath, bool* arrBricksData, int nCurShapeID, int nNextShapeID, int nCurScore);

	// Load tetris data
	bool LoadTetrisData(const char* szPath, bool* arrBricksData, int& nCurShapeID, int& nNextShapeID, int& nCurScore);

	//////////////////////////////////////////////////////////////////////////
	// On timer
	virtual void OnTimer(int nTimerID);

private:
    // Load game animations
    bool __LoadGameAnim();

	// Load high score data
	bool __LoadHighScore();

	// Load strings
	bool __LoadStrings();

private:
    struct _TAnimData
    {
        bool arrBrickState[ROW_COUNT * COLUMN_COUNT];
	};

	typedef map<int, _TAnimData>             TMap_FrameAnimData;
	typedef map<int, TMap_FrameAnimData>     TMap_GameAnimData;

	enum
	{
		enTimerID_SaveData = 1,
		enInterval_SaveData = 5000,
	};

private:
    // All game anim data
    TMap_GameAnimData		m_mapGameAnimData;

	// High score
	int						m_arrHighScore[GAMEID_MAX];

	// Strings
	string					m_arrStrings[STRID_MAX][LANG_MAX];
};