#include "stdafx.h"
#include "FlappybirdGame.h"


void CFlappybirdGame::Start()
{
	CGameBase::Start();

	m_stBirdData.nRowIdx = (ROW_COUNT - 1) / 2;
	m_stBirdData.nInterval = 0;
	m_stBirdData.nTotalInterval = 0;
	m_stBirdData.nUpInterval = 0;
	m_stBirdData.bControlUpFlag = false;
	m_stBirdData.nPassPillarCount = 0;

	// Init pillar
	for (int nIndex = 0; nIndex < PILLAR_COUNT_MAX; ++nIndex)
	{
		_TPillarData& stPillarData = m_stPillarsData.arrPillarData[nIndex];
		stPillarData.nColIdx = -1;
		stPillarData.nLen = -1;
	}
	__CreatePillar(0, COLUMN_COUNT + 1);

	// Update immediately
	__UpdateAllBricksState();
}


void CFlappybirdGame::Update(float dt)
{
	bool bUpdateFlag = false;

	switch (m_enGameStage)
	{
	case STAGE_NORMAL:
		{
			__BirdMove(dt, bUpdateFlag);
			__PillarMove(dt, bUpdateFlag);
			__UpdateGameState();
		}
		break;

	case STAGE_FAIL:
		UpdateBoomAnim(dt, bUpdateFlag);
		break;

	default:
		break;
	}

	if (bUpdateFlag)
	{
		__UpdateAllBricksState();
	}
}


EnGameID CFlappybirdGame::GetGameID()
{
	return GAMEID_FLAPPYBIRD;
}


void CFlappybirdGame::OnButtonEvent(const SEventContextButton* pButtonEvent)
{
	if (m_enGameStage != STAGE_NORMAL)
	{
		return;
	}

	if (pButtonEvent->nButtonID == BTNID_FIRE
		|| pButtonEvent->nButtonID == BTNID_UP)
	{
		m_stBirdData.bControlUpFlag = pButtonEvent->bPressedFlag;
		if (pButtonEvent->bPressedFlag)
		{
			m_stBirdData.nUpInterval = ceil(1000.0f / BIRD_UP_SPEED * 0.5f);
		}
	}
}


void CFlappybirdGame::__PillarMove(float dt, bool& bUpdateFlag)
{
	m_stPillarsData.nInterval += dt;
	if (m_stPillarsData.nInterval < __GetPillarMoveInterval())
	{
		return;
	}

	// reset
	m_stPillarsData.nInterval = 0;
	bUpdateFlag = true;

	// Move
	int nCreatePillarIdx = PILLAR_COUNT_MAX;
	int nMaxColIdx = 0;
	for (int nIndex = 0; nIndex < PILLAR_COUNT_MAX; ++nIndex)
	{
		_TPillarData& stPillarData = m_stPillarsData.arrPillarData[nIndex];
		if (--stPillarData.nColIdx < 0)
		{
			nCreatePillarIdx = nIndex;
			continue;
		}

		if (stPillarData.nColIdx == BIRD_COLUMN_IDX - 1)
		{
			AddScore(PILLAR_PASS_ADD_SCORE);
			if (++m_stBirdData.nPassPillarCount >= UPDATE_SPEED_NEED_PILLAR_COUNT)
			{
				UpdateSpeed(true);
				m_stBirdData.nPassPillarCount = 0;
			}
		}

		nMaxColIdx = max(nMaxColIdx, stPillarData.nColIdx);
	}

	// Create pillar
	if (nCreatePillarIdx >= 0 && nCreatePillarIdx < PILLAR_COUNT_MAX)
	{
		__CreatePillar(nCreatePillarIdx, nMaxColIdx + PILLAR_COLUMN_DISTANCE);
	}
}


void CFlappybirdGame::__CreatePillar(int nIndex, int nColIdx)
{
	if (nIndex < 0 || nIndex >= PILLAR_COUNT_MAX)
	{
		return;
	}

	_TPillarData& stPillarData = m_stPillarsData.arrPillarData[nIndex];
	stPillarData.nLen = Random(1, PILLAR_LEN_MAX);
	stPillarData.nColIdx = nColIdx;
}


void CFlappybirdGame::__BirdMove(float dt, bool& bUpdateFlag)
{
	const float fFactor = 1.5f;

	int nDownColCount = 0;
	if (m_stBirdData.bControlUpFlag)
	{
		m_stBirdData.nTotalInterval = ceil(__GetBirdMoveInterval() / fFactor);
		m_stBirdData.nUpInterval += dt;
		int nUpCount = floor(m_stBirdData.nUpInterval * BIRD_UP_SPEED / 1000.0f);
		if (nUpCount > 0)
		{
			m_stBirdData.nUpInterval = 0;
		}
		nDownColCount -= nUpCount;
	}

	m_stBirdData.nInterval += dt;
	if (m_stBirdData.nInterval >= m_stBirdData.nTotalInterval)
	{
		m_stBirdData.nInterval = 0;
		m_stBirdData.nTotalInterval = ceil(m_stBirdData.nTotalInterval / fFactor);
		nDownColCount += 1;
	}

	if (nDownColCount != 0)
	{
		m_stBirdData.nRowIdx += nDownColCount;
		if (m_stBirdData.nRowIdx < 0)
		{
			m_stBirdData.nRowIdx = 0;
		}
		if (m_stBirdData.nRowIdx >= ROW_COUNT)
		{
			m_stBirdData.nRowIdx = ROW_COUNT - 1;
		}
		bUpdateFlag = true;
	}
}


void CFlappybirdGame::__UpdateAllBricksState()
{
	for (int nBrickID = 0; nBrickID < ROW_COUNT * COLUMN_COUNT; ++nBrickID)
	{
		UpdateBrickState(nBrickID, false);
	}

	// Draw pillar
	for (int nIndex = 0; nIndex < PILLAR_COUNT_MAX; ++nIndex)
	{
		_TPillarData& stPillarData = m_stPillarsData.arrPillarData[nIndex];
		if (stPillarData.nColIdx >= COLUMN_COUNT
			|| stPillarData.nColIdx < 0)
		{
			continue;
		}

		// Top part
		for (int nRowIdx = 0; nRowIdx < stPillarData.nLen; ++nRowIdx)
		{
			UpdateBrickState(GET_BRICKID(nRowIdx, stPillarData.nColIdx), true);
		}

		// Bottom part
		int nBottomLen = __GetPillarLenMax() - stPillarData.nLen;
		for (int nRowIdx = ROW_COUNT - nBottomLen; nRowIdx < ROW_COUNT; ++nRowIdx)
		{
			UpdateBrickState(GET_BRICKID(nRowIdx, stPillarData.nColIdx), true);
		}
	}

	// Draw bird
	if (!DrawBoom())
	{
		UpdateBrickState(GET_BRICKID(m_stBirdData.nRowIdx, BIRD_COLUMN_IDX), true);
	}

	UpdateBrickState(0, GetBrickState(0), true);
}


void CFlappybirdGame::__UpdateGameState()
{
	// Check game over
	if (m_stBirdData.nRowIdx == 0
		|| m_stBirdData.nRowIdx == ROW_COUNT - 1)
	{
		POSITION stPos = { m_stBirdData.nRowIdx, BIRD_COLUMN_IDX };
		GameOverWithBoomAnim(stPos);
		return;
	}

	for (int nIndex = 0; nIndex < PILLAR_COUNT_MAX; ++nIndex)
	{
		_TPillarData& stPillarData = m_stPillarsData.arrPillarData[nIndex];
		if (stPillarData.nColIdx != BIRD_COLUMN_IDX)
		{
			continue;
		}

		int nBottomLen = __GetPillarLenMax() - stPillarData.nLen;
		if ((m_stBirdData.nRowIdx >= 0 && m_stBirdData.nRowIdx < stPillarData.nLen)
			|| (m_stBirdData.nRowIdx >= ROW_COUNT - nBottomLen && m_stBirdData.nRowIdx < ROW_COUNT))
		{
			POSITION stPos = { m_stBirdData.nRowIdx, BIRD_COLUMN_IDX };
			GameOverWithBoomAnim(stPos);
			return;
		}
	}
}


int CFlappybirdGame::__GetBirdMoveInterval()
{
	return BIRD_DOWN_INTERVAL - 3 * m_nSpeed;
}


int CFlappybirdGame::__GetPillarMoveInterval()
{
	return PILLAR_MOVE_INTERVAL - m_nSpeed * 30;
}


int CFlappybirdGame::__GetPillarLenMax()
{
	return PILLAR_LEN_MAX + m_nLevel / 3;
}
