#include "stdafx.h"
#include "OverGame.h"


void COverGame::Start()
{
	// Clear all bricks state
	INIT_BOOL_ARRAY(m_arrBrickState);

	// Other variables
	m_enDirection = DIR_RIGHT;
	m_nColIdx = -1;
	m_nRowIdx = 0;
}


void COverGame::Update(float dt)
{
	// Move
	switch (m_enDirection)
	{
	case DIR_RIGHT:
		__OnMoveRight();
		break;

	case DIR_DOWN:
		__OnMoveDown();
		break;

	case DIR_LEFT:
		__OnMoveLeft();
		break;

	case DIR_UP:
		__OnMoveUp();
		break;

	default:
		break;
	}

    // If the current brick is already set to display, end the update route,
    // and switch to the select game.
    int nBrickID = GET_BRICKID(m_nRowIdx, m_nColIdx);
    if (GetBrickState(nBrickID))
    {
        __SwitchToSelectGame();
        return;
    }
    
    // Update the state of current brick.
    UpdateBrickState(nBrickID, true, true);
}


EnGameID COverGame::GetGameID()
{
    return GAMEID_OVER;
}


void COverGame::OnButtonEvent(const SEventContextButton* pButtonEvent)
{
	__SwitchToSelectGame();
}


void COverGame::__OnMoveRight()
{
    int nBrickID = GET_BRICKID(m_nRowIdx, ++m_nColIdx);
    if (m_nColIdx == COLUMN_COUNT || GetBrickState(nBrickID))
    {
        --m_nColIdx;
		++m_nRowIdx;
        m_enDirection = DIR_DOWN;
    }
}


void COverGame::__OnMoveDown()
{
    int nBrickID = GET_BRICKID(++m_nRowIdx, m_nColIdx);
    if (m_nRowIdx == ROW_COUNT || GetBrickState(nBrickID))
    {
        --m_nRowIdx;
		--m_nColIdx;
        m_enDirection = DIR_LEFT;
    }
}


void COverGame::__OnMoveLeft()
{
    int nBrickID = GET_BRICKID(m_nRowIdx, --m_nColIdx);
    if (m_nColIdx < 0 || GetBrickState(nBrickID))
    {
        ++m_nColIdx;
		--m_nRowIdx;
        m_enDirection = DIR_UP;
    }
}


void COverGame::__OnMoveUp()
{
    int nBrickID = GET_BRICKID(--m_nRowIdx, m_nColIdx);
    if (m_nRowIdx < 0 || GetBrickState(nBrickID))
    {
        ++m_nRowIdx;
		++m_nColIdx;
        m_enDirection = DIR_RIGHT;
    }
}


void COverGame::__SwitchToSelectGame()
{
	SEventContextGameStart stContext;
	stContext.nGameID = GAMEID_SELECT;
	stContext.nSpeed = 0;
	stContext.nLevel = 0;
	stContext.nLife = 0;
	stContext.nScore = 0;
	g_oEventEngine.FireEvent(EVENT_GAME_START, (const char*)&stContext, sizeof(stContext));
}
