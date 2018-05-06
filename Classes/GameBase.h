#pragma once
#include "GameLogic.h"

class CGameBase
{
public:
    CGameBase();
    virtual ~CGameBase();

    // Start
    virtual void Start();

    // End
    virtual void End();

    // Update
    // param: dt, millisecond
    virtual void Update(float dt);

    // Get game id
    virtual EnGameID GetGameID();

    // Button event
    virtual void OnButtonEvent(const SEventContextButton* pButtonEvent);

	// Save data
	virtual bool SaveData(bool bForceFlag);

public:
    // Import context
    void ImportContext(const SEventContextGameStart* pGameStart);

    // Get brick state
    bool GetBrickState(int nBrickID);

    // Get small brick state
    bool GetSmallBrickState(int nBrickID);

    // Update brick state
    // bUpdateFlag: if true, update immediately
    void UpdateBrickState(int nBrickID, bool bState, bool bUpdateFlag = false);

    // Update small brick state
    // bUpdateFlag: if true, update immediately
    void UpdateSmallBrickState(int nBrickID, bool bState, bool bUpdateFlag = false);

    // Update all brick state
    void UpdateAllBrickState();

    // Update all small brick state
    void UpdateAllSmallBrickState();

    // Random: [nLow, nHigh)
    int Random(int nLow, int nHigh);

	// Update game data
	void UpdateGameData(int nGameID = GAMEID_MAX);

	// Update current score and max score
	void AddScore(int nAddScore);

	// Update speed (and level)
	void UpdateSpeed(bool bUpdateFlag);

	// Update small bricks with life
	void InitSmallBricks();

	// Add score in pass stage
	void AddScoreInPassStage(float dt, int nAddScore);

	// Update boom state
	void UpdateBoomAnim(float dt, bool& bUpdateFlag);

	// Init boom anim
	void GameOverWithBoomAnim(const POSITION& stPos);

	// Draw boom
	bool DrawBoom();

	// Get next position
	// If the next pos is invalid, return false
	bool GetNextPos(POSITION& stPos, int nDir);

	// Play sound
	void PlayEffect(const char* szSound);

	// Play BGM
	void PlayBGM(bool bPlayFlag = true, bool bLoopFlag = true);

	// Vibrate
	void Vibrate(bool bShortFlag = false);

	// Switch to the over game
	void SwitchToOverGame();

protected:
    // Brick state
    bool					m_arrBrickState[ROW_COUNT * COLUMN_COUNT];

    // Small brick state
    bool					m_arrSmallBrickState[SMALL_BRICK_COUNT];

    // Waiting for updated bricks
	TMap_BrickState			m_mapUpdateBricks;

    // Waiting for updated small bricks
	TMap_BrickState			m_mapUpdateSmallBricks;

	// Current game stage
	EnGameStage				m_enGameStage;

	// Boom anim data
	TBoomAnimData			m_stBoomAnimData;

	// Add score data
	TPassAddScoreData		m_stAddScoreData;

	// Current level
	int						m_nLevel;

	// Current speed
	int						m_nSpeed;

	// Current score
	int						m_nCurScore;

	// Life
	int						m_nLife;
};