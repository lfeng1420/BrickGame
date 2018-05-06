#include "stdafx.h"
#include "FroggerGame.h"

// River shape
static const bool RIVER_SHAPE[CFroggerGame::RIVER_COUNT][CFroggerGame::RIVER_COLUMN_COUNT] =
{
	{ false, true, true, true, false, false, true, true, true, false, false, true, true, true, false, false, true, true, true, false },
	{ true, true, true, false, false, false, false, false, true, true, true, true, true, false, false, false, false, false, true, true },
	{ false, false, true, true, true, false, false, true, true, false, false, false, true, true, true, false, false, true, true, false },
	{ false, false, false, false, true, true, true, true, false, false, false, false, false, false, true, true, true, true, false, false },
	{ false, false, true, true, true, false, false, false, false, false, false, false, true, true, true, false, false, false, false, false },
};


void CFroggerGame::Start()
{
	CGameBase::Start();

	// Init rivers
	__InitRivers();

	// Init self
	m_stSelfData.bVisibleFlag = true;
	m_stSelfData.nInterval = 0;
	m_stSelfData.stPos.x = min(ROW_COUNT - 1, __GetStartRowIdx() + RIVER_COUNT * 2);
	m_stSelfData.stPos.y = (COLUMN_COUNT - 1) / 2;
	m_stSelfData.nPassCount = 0;

	// Clear all succ dots
	m_lsSuccDotPos.clear();

	// Update immediately
	__UpdateAllBricksState();
}


void CFroggerGame::Update(float dt)
{
	bool bUpdateFlag = false;

	switch (m_enGameStage)
	{
	case STAGE_FAIL:
		UpdateBoomAnim(dt, bUpdateFlag);
		break;

	case STAGE_PASS:
	{
		// loop add score util the count is arrived
		AddScoreInPassStage(dt, PASS_ONE_ADD_SCORE);
		return;
	}
	break;

	case STAGE_NORMAL:
	{
		__UpdateRivers(dt, bUpdateFlag);
		__UpdateSelf(dt, bUpdateFlag);
	}
	break;

	default:
		return;
		break;
	}

	if (bUpdateFlag)
	{
		__UpdateAllBricksState();
	}
}


EnGameID CFroggerGame::GetGameID()
{
	return GAMEID_FROGGER;
}


void CFroggerGame::OnButtonEvent(const SEventContextButton* pButtonEvent)
{
	if (m_enGameStage != STAGE_NORMAL 
		|| !pButtonEvent->bPressedFlag)
	{
		return;
	}

	int nBtnID = ((pButtonEvent->nButtonID == BTNID_FIRE) ? BTNID_UP : pButtonEvent->nButtonID);
	if (nBtnID >= BTNID_DIRMAX)
	{
		return;
	}

	// Get next pos
	POSITION stNextPos = m_stSelfData.stPos;
	int nDir = BTNID_2_DIR[nBtnID];
	if (!__GetNextPos(stNextPos, nDir))
	{
		return;
	}

	// Update old pos
	UpdateBrickState(GET_BRICKIDBYPOS(m_stSelfData.stPos), false, false);
	m_stSelfData.stPos = stNextPos;
	// Update current pos
	UpdateBrickState(GET_BRICKIDBYPOS(m_stSelfData.stPos), false, true);
	// Check game over
	if (__CheckGameOver())
	{
		GameOverWithBoomAnim(m_stSelfData.stPos);
		return;
	}

	if (m_stSelfData.stPos.x == max(__GetTopRowIdx(), 0))
	{
		m_lsSuccDotPos.push_back(m_stSelfData.stPos);
		// Add score
		AddScore(PASS_ONE_ADD_SCORE);
		// Reset self
		m_stSelfData.stPos.x = min(ROW_COUNT - 1, __GetStartRowIdx() + RIVER_COUNT * 2);
		m_stSelfData.stPos.y = (COLUMN_COUNT - 1) / 2;
		// Check pass condition
		if (++m_stSelfData.nPassCount >= PASS_REQUIRE_COUNT)
		{
			// Update last time
			__UpdateAllBricksState();
			// Go to the pass stage
			m_enGameStage = STAGE_PASS;
		}
	}
}


void CFroggerGame::__InitRivers()
{
	bool bLeftFlag = (Random(0, 100) < 50);
	int nStartRowIdx = __GetStartRowIdx();

	// first sep
	for (int nColIdx = 0; nColIdx < COLUMN_COUNT; ++nColIdx)
	{
		int nBrickID = GET_BRICKID(nStartRowIdx - 1, nColIdx);
		UpdateBrickState(nBrickID, true);
	}

	// Random river index
	vector<int> vecRiverIdx;
	for (int nIdex = 0; nIdex < RIVER_COUNT; ++nIdex)
	{
		vecRiverIdx.push_back(nIdex);
	}
	std::random_shuffle(vecRiverIdx.begin(), vecRiverIdx.end());

	for (int nRowIdx = nStartRowIdx, nIndex = 0; nRowIdx < min(nStartRowIdx + RIVER_COUNT * 2, ROW_COUNT) && nIndex < RIVER_COUNT; nRowIdx += 2, ++nIndex)
	{
		_TRiverData& stRiverData = m_arrRiverData[nIndex];
		stRiverData.bLeftFlag = bLeftFlag;
		stRiverData.nOffset = vecRiverIdx[nIndex];
		stRiverData.nRiverIdx = nIndex;
		stRiverData.nRowIdx = nRowIdx;
		stRiverData.nMoveInterval = 0;
		bLeftFlag = !bLeftFlag;

		// sep
		for (int nColIdx = 0; nColIdx < COLUMN_COUNT; ++nColIdx)
		{
			int nBrickID = GET_BRICKID(nRowIdx + 1, nColIdx);
			UpdateBrickState(nBrickID, true);
		}
	}
}


void CFroggerGame::__UpdateRivers(float dt, bool& bUpdateFlag)
{
	std::vector<int> vecUpdateRiverIdx;
	for (int nIndex = 0; nIndex < RIVER_COUNT; ++nIndex)
	{
		vecUpdateRiverIdx.push_back(nIndex);
	}

	// random
	std::random_shuffle(vecUpdateRiverIdx.begin(), vecUpdateRiverIdx.end());

	// Random udpate river count
	int nUpdateRiverCount = Random(UPDATE_RIVER_COUNT_MIN, RIVER_COUNT);
	
	// Update
	for (int nUpdateIdx = 0; nUpdateIdx < nUpdateRiverCount && nUpdateIdx < RIVER_COUNT; ++nUpdateIdx)
	{
		int nRiverIdx = vecUpdateRiverIdx[nUpdateIdx];
		_TRiverData& stRiverData = m_arrRiverData[nRiverIdx];
		stRiverData.nMoveInterval += dt;
		if (stRiverData.nMoveInterval < __GetRiverUpdateInterval())
		{
			continue;
		}

		stRiverData.nMoveInterval = 0;
		stRiverData.nOffset += (stRiverData.bLeftFlag ? 1 : -1);
		if (stRiverData.nOffset <= -RIVER_COLUMN_COUNT)
		{
			stRiverData.nOffset = 0;
		}
		else if (stRiverData.nOffset >= RIVER_COLUMN_COUNT)
		{
			stRiverData.nOffset = 0;
		}

		if (__CheckGameOver())
		{
			GameOverWithBoomAnim(m_stSelfData.stPos);
		}

		// Need update
		bUpdateFlag = true;
	}
}


void CFroggerGame::__UpdateSelf(float dt, bool& bUpdateFlag)
{
	m_stSelfData.nInterval += dt;
	if (m_stSelfData.nInterval < SELF_UPDATE_INTERVAL)
	{
		return;
	}

	// Reset
	m_stSelfData.nInterval = 0;
	m_stSelfData.bVisibleFlag = !m_stSelfData.bVisibleFlag;
	bUpdateFlag = true;
}


void CFroggerGame::__UpdateAllBricksState()
{
	// Draw rivers
	for (int nIndex = 0; nIndex < RIVER_COUNT; ++nIndex)
	{
		const _TRiverData& stRiverData = m_arrRiverData[nIndex];
		int nOffset = ((stRiverData.nOffset < 0) ? (RIVER_COLUMN_COUNT + stRiverData.nOffset) : stRiverData.nOffset);
		const bool* arrRiverShape = RIVER_SHAPE[stRiverData.nRiverIdx];
		int nColIdx = 0;
		for (int nIdx = nOffset; nIdx < RIVER_COLUMN_COUNT && nColIdx < COLUMN_COUNT; ++nIdx, ++nColIdx)
		{
			UpdateBrickState(GET_BRICKID(stRiverData.nRowIdx, nColIdx), arrRiverShape[nIdx]);
		}
		for (int nIdx = 0; nColIdx < COLUMN_COUNT; ++nIdx, ++nColIdx)
		{
			UpdateBrickState(GET_BRICKID(stRiverData.nRowIdx, nColIdx), arrRiverShape[nIdx]);
		}

	}

	// Draw succ dot
	FOREACH_IN_CONST_CONTAINER(TList_SuccDotPos, m_lsSuccDotPos, itDotPos)
	{
		UpdateBrickState(GET_BRICKIDBYPOS(*itDotPos), true);
	}

	// Draw boom or self
	if (!DrawBoom())
	{
		UpdateBrickState(GET_BRICKIDBYPOS(m_stSelfData.stPos), m_stSelfData.bVisibleFlag);
	}

	// Update immediately
	UpdateBrickState(0, GetBrickState(0), true);
}


int CFroggerGame::__GetRiverUpdateInterval()
{
	return RIVER_UPDATE_INTERVAL - 40 * m_nSpeed;
}


bool CFroggerGame::__GetNextPos(POSITION& stPos, int nDir)
{
	switch (nDir)
	{
	case DIR_RIGHT:
		++stPos.y;
		break;

	case DIR_LEFT:
		--stPos.y;
		break;

	case DIR_UP:
		stPos.x -= 2;
		break;

	case DIR_DOWN:
		stPos.x += 2;
		break;

	default:
		return false;
		break;
	}

	if (stPos.x < 0 || stPos.x >= ROW_COUNT
		|| stPos.y < 0 || stPos.y >= COLUMN_COUNT)
	{
		return false;
	}

	return true;
}


bool CFroggerGame::__CheckGameOver()
{
	// Check current column
	for (int nIndex = 0; nIndex < RIVER_COUNT; ++nIndex)
	{
		const _TRiverData& stRiverData = m_arrRiverData[nIndex];
		if (m_stSelfData.stPos.x != stRiverData.nRowIdx)
		{
			continue;
		}

		int nOffset = ((stRiverData.nOffset < 0) ? (RIVER_COLUMN_COUNT + stRiverData.nOffset) : stRiverData.nOffset);
		const bool* arrRiverShape = RIVER_SHAPE[stRiverData.nRiverIdx];
		int nColIdx = 0;
		for (int nIdx = nOffset; nIdx < RIVER_COLUMN_COUNT && nColIdx < COLUMN_COUNT; ++nIdx, ++nColIdx)
		{
			if (arrRiverShape[nIdx] && nColIdx == m_stSelfData.stPos.y)
			{
				return true;
			}
		}
		for (int nIdx = 0; nColIdx < COLUMN_COUNT; ++nIdx, ++nColIdx)
		{
			if (arrRiverShape[nIdx] && nColIdx == m_stSelfData.stPos.y)
			{
				return true;
			}
		}
	}

	// Check top pos
	FOREACH_IN_CONST_CONTAINER(TList_SuccDotPos, m_lsSuccDotPos, itDotPos)
	{
		if (EQUAL_POS(*itDotPos, m_stSelfData.stPos))
		{
			return true;
		}
	}

	return false;
}


int CFroggerGame::__GetStartRowIdx()
{
	return RIVER_START_ROWIDX - m_nLevel / 2;
}


int CFroggerGame::__GetTopRowIdx()
{
	return RIVER_TOP_ROWIDX - m_nLevel / 2;
}
