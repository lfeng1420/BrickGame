#pragma once
#include "GameBase.h"

class CSelectGame : public CGameBase
{
public:
    // Start
    void Start();

    // Update
    // param: dt, millisecond
    void Update(float dt);

    // Get game id
    EnGameID GetGameID();

    // Button event
    void OnButtonEvent(const SEventContextButton* pButtonEvent);

private:
    // Start current select game
    void __StartGame();

	// Update game anim
	void __UpdateGameAnim(float dt, bool& bUpdateFlag);

	// Update current game
	void __UpdateGameID(float dt, bool& bUpdateFlag);


private:
	enum
	{
		PLAY_ANIM_INTERVAL = 1000,
		UPDATE_GAME_INTERVAL = 120,
	};

private:
    // Current select game id
    int     m_nGameID;

    // Current anim frame
    int     m_nFrameIdx;

    // Current interval
    int     m_nCurInterval;

	// Update game interval
	int		m_nUpdateGameInterval;
};