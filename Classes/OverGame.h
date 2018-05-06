#pragma once
#include "GameBase.h"

class COverGame : public CGameBase
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
    // Move right
    void __OnMoveRight();

    // Move down
    void __OnMoveDown();

    // Move left
    void __OnMoveLeft();

    // Move up
    void __OnMoveUp();

	// Switch to the select game
	void __SwitchToSelectGame();

private:
    // Current column value
    int             m_nColIdx;

    // Current row value
    int             m_nRowIdx;

    // Move direction
    EnDirection    m_enDirection;

    // End flag
    bool            m_bEndFlag;
};