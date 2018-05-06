#include "stdafx.h"
#include "MatchGame.h"

static const int SHAPE_ROW_COUNT = 2;
static const int SHAPE_COL_COUNT = 2;
static const bool MATCH_SHAPE[][SHAPE_ROW_COUNT * SHAPE_COL_COUNT] =
{
	{ false, false, true, false, },
	{ true, false, true, false, },
	{ true, false, true, true, },
	{ true, true, true, true, },
};


void CMatchGame::Start()
{
	CGameBase::Start();

	// Initialize shapes
	__InitSrcDestShapes();

	// Initialize match succ count
	m_nMatchSuccCount = 0;

	// Update now
	__UpdateAllBricksState();
}


void CMatchGame::Update(float dt)
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
			AddScoreInPassStage(dt, SHAPE_MATCH_ADD_SCORE);
			return;
		}
		break;

	case STAGE_NORMAL:
		{
			bUpdateFlag = __UpdateShapes(dt);
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


EnGameID CMatchGame::GetGameID()
{
	return GAMEID_MATCH;
}


void CMatchGame::OnButtonEvent(const SEventContextButton* pButtonEvent)
{
	if (m_enGameStage != STAGE_NORMAL 
		|| !pButtonEvent->bPressedFlag)
	{
		return;
	}

	if (pButtonEvent->nButtonID == BTNID_FIRE)
	{
		// Ready match
		m_stSrcShape.nInterval = 0;
		return;
	}

	if (pButtonEvent->nButtonID < BTNID_DIRMAX)
	{
		const int BTNID_2_SHAPEIDX[] = {2, SHAPE_COUNT_MAX, 0, 1};
		int nShapeIdx = BTNID_2_SHAPEIDX[pButtonEvent->nButtonID];
		if (nShapeIdx < 0 || nShapeIdx >= SHAPE_COUNT_MAX)
		{
			return;
		}

		int& nShapeID = m_stSrcShape.arrShapeID[nShapeIdx];
		if (++nShapeID >= _countof(MATCH_SHAPE))
		{
			nShapeID = 0;
		}

		__UpdateShapes(__GetShapeMoveInterval(false) - m_stSrcShape.nInterval);
		__UpdateAllBricksState();
	}
}


void CMatchGame::__InitSrcDestShapes()
{
	// Init dest shape data
	m_stDestShape.nRowIdx = 0;
	m_stDestShape.nInterval = 0;
	__RandomShapes(m_stDestShape.arrShapeID);

	// Init src shape data
	m_stSrcShape.nRowIdx = ROW_COUNT - SHAPE_ROW_COUNT;
	m_stSrcShape.nInterval = -1;
	__RandomShapes(m_stSrcShape.arrShapeID);
}


void CMatchGame::__RandomShapes(int* arrShapes)
{
	if (_countof(MATCH_SHAPE) < SHAPE_COUNT_MAX)
	{
		return;
	}

	vector<int> vecShapes;
	for (int nIndex = 0; nIndex < _countof(MATCH_SHAPE); ++nIndex)
	{
		vecShapes.push_back(nIndex);
	}

	std::random_shuffle(vecShapes.begin(), vecShapes.end());

	for (int nIndex = 0; nIndex < SHAPE_COUNT_MAX; ++nIndex)
	{
		arrShapes[nIndex] = vecShapes[nIndex];
	}
}


void CMatchGame::__UpdateAllBricksState()
{
	for (int nBrickID = 0; nBrickID < ROW_COUNT * COLUMN_COUNT; ++nBrickID)
	{
		UpdateBrickState(nBrickID, false);
	}

	// Draw shapes
	int nSepBrickCount = (COLUMN_COUNT - SHAPE_COUNT_MAX * SHAPE_COL_COUNT) / (SHAPE_COUNT_MAX + 1);
	for (int nIndex = 0; nIndex < SHAPE_COUNT_MAX; ++nIndex)
	{
		int nSrcShapeID = m_stSrcShape.arrShapeID[nIndex];
		int nDestShapeID = m_stDestShape.arrShapeID[nIndex];
		int nColIdx = SHAPE_COL_COUNT * nIndex + (nIndex + 1) * nSepBrickCount;
		for (int nBrickID = 0; nBrickID < SHAPE_ROW_COUNT * SHAPE_COL_COUNT; ++nBrickID)
		{
			int nRowOffset = nBrickID / SHAPE_COL_COUNT;
			int nColOffset = nBrickID % SHAPE_COL_COUNT;
			UpdateBrickState(GET_BRICKID(m_stSrcShape.nRowIdx + nRowOffset, nColIdx + nColOffset), MATCH_SHAPE[nSrcShapeID][nBrickID]);
			UpdateBrickState(GET_BRICKID(m_stDestShape.nRowIdx + nRowOffset, nColIdx + nColOffset), MATCH_SHAPE[nDestShapeID][nBrickID]);
		}
	}

	// Draw boom
	DrawBoom();

	// Update immediately
	UpdateBrickState(0, GetBrickState(0), true);
}


bool CMatchGame::__UpdateShapes(float dt, bool bSelfOnlyFlag /*= false*/)
{
	bool bUpdateFlag = false;

	if (m_stSrcShape.nInterval >= 0)
	{
		bUpdateFlag = true;
		m_stSrcShape.nInterval += dt;
		if (m_stSrcShape.nInterval >= __GetShapeMoveInterval(false))
		{
			m_stSrcShape.nInterval = 0;
			if (--m_stSrcShape.nRowIdx <= m_stDestShape.nRowIdx)
			{
				__HandleMatchResult();
			}
		}
	}

	if (!bSelfOnlyFlag)
	{
		m_stDestShape.nInterval += dt;
		if (m_stDestShape.nInterval >= __GetShapeMoveInterval(true))
		{
			bUpdateFlag = true;
			m_stDestShape.nInterval = 0;
			if (++m_stDestShape.nRowIdx > (ROW_COUNT - SHAPE_ROW_COUNT * 2))
			{
				__HandleMatchResult();
			}
		}
	}

	return bUpdateFlag;
}


int CMatchGame::__GetShapeMoveInterval(bool bDestShapeFlag)
{
	return (bDestShapeFlag ? (DEST_SHAPE_MOVE_INTERVAL - 70 * m_nSpeed) : SRC_SHAPE_MOVE_INTERVAL);
}


int CMatchGame::__GetDismatchShapeIdx()
{
	for (int nIndex = 0; nIndex < SHAPE_COUNT_MAX; ++nIndex)
	{
		if (m_stDestShape.arrShapeID[nIndex] != m_stSrcShape.arrShapeID[nIndex])
		{
			return nIndex;
		}
	}

	return SHAPE_COUNT_MAX;
}


void CMatchGame::__HandleMatchResult()
{
	int nDisMatchIdx = __GetDismatchShapeIdx();
	if (nDisMatchIdx != SHAPE_COUNT_MAX)
	{
		int nSepBrickCount = (COLUMN_COUNT - SHAPE_COUNT_MAX * SHAPE_COL_COUNT) / (SHAPE_COUNT_MAX + 1);
		POSITION stPos = { m_stDestShape.nRowIdx, SHAPE_COL_COUNT * nDisMatchIdx + (nDisMatchIdx + 1) * nSepBrickCount };
		GameOverWithBoomAnim(stPos);
		return;
	}

	// Add score
	AddScore(SHAPE_MATCH_ADD_SCORE);
	// Start next match
	__InitSrcDestShapes();
	// Add match succ count
	if (++m_nMatchSuccCount >= REQUIRE_MATCH_SUCC_COUNT)
	{
		m_enGameStage = STAGE_PASS;
		return;
	}
}
