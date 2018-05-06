#include "stdafx.h"
#include "PinballGame.h"


void CPinballGame::Start()
{
	CGameBase::Start();

	// Init guard pos
	m_stGuardData.nDir = DIR_MAX;
	m_stGuardData.nMoveInterval = 0;
	POSITION stGuardPos = { GUARD_ROW_IDX, GUARD_COLUMN_IDX };
	__UpdateGuardPos(stGuardPos, false);

	// Init ball data
	m_stBallData.nMoveInterval = -1;
	m_stBallData.stIncr.x = -1;
	m_stBallData.stIncr.y = 1;
	POSITION stBallPos = { GUARD_ROW_IDX - 1, (COLUMN_COUNT - 1) / 2 };
	__UpdateBallPos(stBallPos, false);

	// Init blocks
	__InitBlocks();

	// Update immediately
	UpdateBrickState(0, GetBrickState(0), true);
}


void CPinballGame::Update(float dt)
{
	bool bUpdateFlag = false;

	switch (m_enGameStage)
	{
	case STAGE_FAIL:
		{
			UpdateBoomAnim(dt, bUpdateFlag);
			DrawBoom();
		}
		break;

	case STAGE_PASS:
		{
			// loop add score util the count is arrived
			AddScoreInPassStage(dt, HIT_BLOCK_ADD_SCORE);
			return;
		}
		break;

	case STAGE_NORMAL:
		{
			__UpdateGuard(dt, bUpdateFlag);
			__UpdateBall(dt, bUpdateFlag);
			UpdateOther(dt, bUpdateFlag);
		}
		break;

	default:
		return;
		break;
	}

	if (bUpdateFlag)
	{
		UpdateBrickState(0, GetBrickState(0), true);
	}
}


EnGameID CPinballGame::GetGameID()
{
	return GAMEID_PINBALL;
}


void CPinballGame::OnButtonEvent(const SEventContextButton* pButtonEvent)
{
	if (m_enGameStage != STAGE_NORMAL)
	{
		return;
	}

	if (pButtonEvent->nButtonID == BTNID_FIRE)
	{
		if (m_stBallData.nMoveInterval < 0)
		{
			m_stBallData.nMoveInterval = 0;
		}
		else
		{
			m_bSpeedUpFlag = pButtonEvent->bPressedFlag;
		}
		
		return;
	}

	if (pButtonEvent->nButtonID >= BTNID_DIRMAX)
	{
		return;
	}
	int nDir = BTNID_2_DIR[pButtonEvent->nButtonID];
	if (nDir != DIR_LEFT && nDir != DIR_RIGHT)
	{
		return;
	}

	if (pButtonEvent->bPressedFlag && nDir != m_stGuardData.nDir)
	{
		m_stGuardData.nDir = nDir;
		bool bUpdateFlag = false;
		__UpdateGuard(__GetGuardMoveInterval() - m_stGuardData.nMoveInterval, bUpdateFlag);
		UpdateBrickState(0, GetBrickState(0), true);
		return;
	}

	if (!pButtonEvent->bPressedFlag && m_stGuardData.nDir == nDir)
	{
		m_stGuardData.nDir = DIR_MAX;
		return;
	}
}


void CPinballGame::UpdateOther(float dt, bool& bUpdateFlag)
{
}


int CPinballGame::GetBlocksStartRowIdx()
{
	return m_nLevel / 2;
}


void CPinballGame::__InitBlocks()
{
	int nTotalBrickCount = ROW_COUNT * COLUMN_COUNT;
	for (int nRowIdx = GetBlocksStartRowIdx(); nRowIdx < GetBlocksStartRowIdx() + BLOCKS_ROW_COUNT; ++nRowIdx)
	{
		for (int nColIdx = 0; nColIdx < COLUMN_COUNT / 2; ++nColIdx)
		{
			UpdateBrickState(GET_BRICKID(nRowIdx, nColIdx), Random(0, nTotalBrickCount) <= nTotalBrickCount / 2);
		}
		for (int nColIdx = COLUMN_COUNT / 2; nColIdx < COLUMN_COUNT; ++nColIdx)
		{
			UpdateBrickState(GET_BRICKID(nRowIdx, nColIdx), GetBrickState(GET_BRICKID(nRowIdx, (COLUMN_COUNT - 1) - nColIdx)));
		}
	}
}


void CPinballGame::__UpdateBall(float dt, bool& bUpdateFlag)
{
	if (m_stBallData.nMoveInterval < 0)
	{
		return;
	}

	m_stBallData.nMoveInterval += dt;
	if (m_stBallData.nMoveInterval < __GetBallMoveInterval())
	{
		return;
	}

	// reset
	m_stBallData.nMoveInterval = 0;
	bUpdateFlag = true;

	// Next pos
	POSITION stNextPos = m_stBallData.stPos;
	stNextPos.x += m_stBallData.stIncr.x;
	stNextPos.y += m_stBallData.stIncr.y;
	
	// Check valid
	bool bNextPosValidFlag = true;
	do 
	{
		if (stNextPos.y < 0 || stNextPos.y >= COLUMN_COUNT)
		{
			if (GetBrickState(GET_BRICKID(stNextPos.x, m_stBallData.stPos.y)))
			{
				UpdateBrickState(GET_BRICKID(stNextPos.x, m_stBallData.stPos.y), false);
				AddScore(HIT_BLOCK_ADD_SCORE);
				m_stBallData.stIncr.x *= -1;
			}
			else if (GetBrickState(GET_BRICKID(stNextPos.x, m_stBallData.stPos.y - m_stBallData.stIncr.y)))
			{
				UpdateBrickState(GET_BRICKID(stNextPos.x, m_stBallData.stPos.y - m_stBallData.stIncr.y), false);
				AddScore(HIT_BLOCK_ADD_SCORE);
				m_stBallData.stIncr.x *= -1;
			}
			else
			{
				// Play effect
				PlayEffect(EFFECT_WALL);
			}

			m_stBallData.stIncr.y *= -1;
			bNextPosValidFlag = false;
		}

		if (stNextPos.x < 0)
		{
			m_stBallData.stIncr.x *= -1;
			bNextPosValidFlag = false;
		}

		if (!bNextPosValidFlag)
		{
			break;
		}

		bool bCurRowNextColFlag = GetBrickState(GET_BRICKID(m_stBallData.stPos.x, stNextPos.y));
		bool bNextRowCurColFlag = GetBrickState(GET_BRICKID(stNextPos.x, m_stBallData.stPos.y));
		if (bCurRowNextColFlag && bNextRowCurColFlag)
		{
			UpdateBrickState(GET_BRICKID(m_stBallData.stPos.x, stNextPos.y), false);
			UpdateBrickState(GET_BRICKID(stNextPos.x, m_stBallData.stPos.y), false);
			AddScore(HIT_BLOCK_ADD_SCORE * 2);
			m_stBallData.stIncr.y *= -1;
			m_stBallData.stIncr.x *= -1;
			bNextPosValidFlag = false;
			break;
		}

		if (bCurRowNextColFlag && !bNextRowCurColFlag)
		{
			UpdateBrickState(GET_BRICKID(m_stBallData.stPos.x, stNextPos.y), false);
			AddScore(HIT_BLOCK_ADD_SCORE);
			m_stBallData.stIncr.y *= -1;
			bNextPosValidFlag = false;
			break;
		}

		if (!bCurRowNextColFlag && bNextRowCurColFlag)
		{
			UpdateBrickState(GET_BRICKID(stNextPos.x, m_stBallData.stPos.y), false);
			AddScore(HIT_BLOCK_ADD_SCORE);
			m_stBallData.stIncr.x *= -1;
			bNextPosValidFlag = false;
			break;
		}
	} while (0);

	// Check game pass
	if (__CheckGamePass())
	{
		m_enGameStage = STAGE_PASS;
	}

	if (!bNextPosValidFlag)
	{
		return;
	}

	// Update pos
	__UpdateBallPos(stNextPos);

	// Edge bounce
	if (m_stBallData.stPos.x == GUARD_ROW_IDX - 1)
	{
		if ((m_stBallData.stPos.y == m_stGuardData.stPos.y - 1
			&& m_stBallData.stIncr.y > 0
			&& m_stBallData.stIncr.x > 0)
			|| (m_stBallData.stPos.y == m_stGuardData.stPos.y + GUARD_COLUMN_COUNT
				&& m_stBallData.stIncr.y < 0
				&& m_stBallData.stIncr.x > 0))
		{
			m_stBallData.stIncr.y *= -1;
			m_stBallData.stIncr.x *= -1;
			// Play effect
			PlayEffect(EFFECT_WALL);
		}
		else if (m_stBallData.stPos.y >= m_stGuardData.stPos.y
			&& m_stBallData.stPos.y < m_stGuardData.stPos.y + GUARD_COLUMN_COUNT)
		{
			m_stBallData.stIncr.x *= -1;
			// Play effect
			PlayEffect(EFFECT_WALL);
		}
	}

	// Check game over
	if (m_stBallData.stPos.x == m_stGuardData.stPos.x)
	{
		GameOverWithBoomAnim(stNextPos);
		return;
	}
}


void CPinballGame::__UpdateGuard(float dt, bool& bUpdateFlag)
{
	m_stGuardData.nMoveInterval += dt;
	if (m_stGuardData.nMoveInterval < __GetGuardMoveInterval())
	{
		return;
	}

	m_stGuardData.nMoveInterval = 0;
	POSITION stNextPos = m_stGuardData.stPos;
	if (!GetNextPos(stNextPos, m_stGuardData.nDir) 
		|| stNextPos.y > COLUMN_COUNT - GUARD_COLUMN_COUNT)
	{
		return;
	}

	__UpdateGuardPos(stNextPos);
	if (m_stBallData.stPos.x == m_stGuardData.stPos.x - 1
		&& m_stBallData.stPos.y >= m_stGuardData.stPos.y
		&& m_stBallData.stPos.y < m_stGuardData.stPos.y + GUARD_COLUMN_COUNT)
	{
		int nBallY = m_stBallData.stPos.y + ((m_stGuardData.nDir == DIR_LEFT) ? -1 : 1);
		POSITION stBallPos = { m_stBallData.stPos.x, nBallY };
		__UpdateBallPos(stBallPos);
	}
	bUpdateFlag = true;
}


void CPinballGame::__UpdateGuardPos(const POSITION& stNewPos, bool bClearOldPosFlag /*= true*/)
{
	if (bClearOldPosFlag)
	{
		for (int nColIdx = m_stGuardData.stPos.y; nColIdx < m_stGuardData.stPos.y + GUARD_COLUMN_COUNT; ++nColIdx)
		{
			UpdateBrickState(GET_BRICKID(m_stGuardData.stPos.x, nColIdx), false);
		}
	}

	for (int nColIdx = stNewPos.y; nColIdx < stNewPos.y + GUARD_COLUMN_COUNT; ++nColIdx)
	{
		UpdateBrickState(GET_BRICKID(stNewPos.x, nColIdx), true);
	}

	m_stGuardData.stPos = stNewPos;
}


void CPinballGame::__UpdateBallPos(const POSITION& stNewPos, bool bClearOldPosFlag /*= true*/)
{
	if (bClearOldPosFlag)
	{
		UpdateBrickState(GET_BRICKIDBYPOS(m_stBallData.stPos), false);
	}

	UpdateBrickState(GET_BRICKIDBYPOS(stNewPos), true);
	m_stBallData.stPos = stNewPos;
}


bool CPinballGame::__CheckGamePass()
{
	for (int nRowIdx = GetBlocksStartRowIdx(); nRowIdx < GetBlocksStartRowIdx() + BLOCKS_ROW_COUNT; ++nRowIdx)
	{
		for (int nColIdx = 0; nColIdx < COLUMN_COUNT; ++nColIdx)
		{
			if (GetBrickState(GET_BRICKID(nRowIdx, nColIdx)))
			{
				return false;
			}
		}
	}

	return true;
}


int CPinballGame::__GetBallMoveInterval()
{
	return (BALL_MOVE_INTERVAL - 6 * m_nSpeed) / (m_bSpeedUpFlag ? 2 : 1);
}


int CPinballGame::__GetGuardMoveInterval()
{
	return GUARD_MOVE_INTERVAL - m_nSpeed;
}
