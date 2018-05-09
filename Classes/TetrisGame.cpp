#include "stdafx.h"
#include "TetrisGame.h"

static const char* TETRIS_DATAPATH = "./tetris.json";


void CTetrisGame::Start()
{
	CGameBase::Start();

	// Check record
	if (LoadTetrisData())
	{
		// Update all bricks
		UpdateAllBrickState();
		// Update score UI
		UpdateGameData();
		RandomShape(true, m_stTetrisData.nShapeID, m_nNextShapeID);
	}
	else
	{
		// Random current shape
		m_nNextShapeID = Random(0, GetShapeCount());
		RandomShape(true);
	}
}


void CTetrisGame::Update(float dt)
{
	if (m_enGameStage == STAGE_FAIL)
	{
		SwitchToOverGame();
		return;
	}

	bool bUpdateFlag = false;
	__TetrisControlMove(dt, bUpdateFlag);
	__TetrisMoveDown(dt, bUpdateFlag);
	UpdateOther(dt, bUpdateFlag);

	if (bUpdateFlag)
	{
		UpdateValidBricksState();
	}
}


EnGameID CTetrisGame::GetGameID()
{
	return GAMEID_TETRIS;
}


void CTetrisGame::OnButtonEvent(const SEventContextButton* pButtonEvent)
{
	if (m_enGameStage != STAGE_NORMAL)
	{
		return;
	}

	// Fire
	if (pButtonEvent->bPressedFlag 
		&& pButtonEvent->nButtonID == BTNID_FIRE)
	{
		int nNextShapeID = __GetNextShapeID();
		POSITION stNextPos = { m_stTetrisData.stPos.x, m_stTetrisData.stPos.y};
		if (stNextPos.y >= COLUMN_COUNT / 2)
		{
			while (stNextPos.y >= COLUMN_COUNT / 2)
			{
				if (CheckShapePos(stNextPos, nNextShapeID))
				{
					UpdateShape(nNextShapeID, stNextPos);
					return;
				}
				--stNextPos.y;
			}
		}
		else
		{
			while (stNextPos.y < COLUMN_COUNT / 2)
			{
				if (CheckShapePos(stNextPos, nNextShapeID))
				{
					UpdateShape(nNextShapeID, stNextPos);
					return;
				}
				++stNextPos.y;
			}
		}
		
		return;
	}

	// Direction button
	if (pButtonEvent->nButtonID >= BTNID_DIRMAX)
	{
		return;
	}

	// Up button
	if (pButtonEvent->nButtonID == BTNID_UP
		&& pButtonEvent->bPressedFlag)
	{
		if (!GET_BOOLVALUE("UPBUTTON", false))
		{
			SEventContextButton stButtonEvent = { BTNID_FIRE, true };
			OnButtonEvent(&stButtonEvent);
		}
		else
		{
			POSITION stNextPos = m_stTetrisData.stPos;
			// Check pos
			while (CheckShapePos(stNextPos, m_stTetrisData.nShapeID))
			{
				++stNextPos.x;
			}

			--stNextPos.x;
			UpdateShape(m_stTetrisData.nShapeID, stNextPos);
			Update(__GetTetrisMoveInterval() - m_stTetrisData.nMoveInterval);
		}

		return;
	}

	// Down button
	if (pButtonEvent->nButtonID == BTNID_DOWN)
	{
		m_stTetrisData.bSpeedUpFlag = pButtonEvent->bPressedFlag;
		return;
	}

	// Left or right button
	int nDir = BTNID_2_DIR[pButtonEvent->nButtonID];
	if (nDir == DIR_LEFT || nDir == DIR_RIGHT)
	{
		if (pButtonEvent->bPressedFlag && m_stTetrisData.nDir != nDir)
		{
			m_stTetrisData.nDir = nDir;
			bool bUpdateFlag = false;
			__TetrisControlMove(CONTROL_MOVE_INTERVAL - m_stTetrisData.nControlInterval, bUpdateFlag);
			// Update immediately
			UpdateBrickState(0, GetBrickState(0), true);
		}
		else if (!pButtonEvent->bPressedFlag && m_stTetrisData.nDir == nDir)
		{
			m_stTetrisData.nDir = DIR_MAX;
		}
	}
}


bool CTetrisGame::SaveData(bool bForceFlag)
{
	return SaveTetrisData(bForceFlag);
}


int CTetrisGame::GetShapeCount()
{
	return TETRIS_NORMAL_SHAPE_COUNT;
}


void CTetrisGame::OnTetrisMoveFinish()
{
	// Sync current shape
	UpdateShape(m_stTetrisData.nShapeID, m_stTetrisData.stPos, true);

	// Check game over
	if (m_stTetrisData.stPos.x == 0)
	{
		// Vibrate
		Vibrate();
		m_enGameStage = STAGE_FAIL;
		return;
	}

	// Remove rows
	RemoveRows();
	// Generate new shape
	RandomShape();
}


bool CTetrisGame::CheckShapePos(const POSITION& stNextPos, int nShapeID)
{
	if (nShapeID < 0 || nShapeID >= TETRIS_SHAPE_COUNT)
	{
		return false;
	}

	int nTetrisRowCount = TETRIS_SHAPE_ROW_COUNT[nShapeID];
	int nTetrisColCount = TETRIS_SHAPE_COL_COUNT[nShapeID];
	for (int nRowIdx = 0; nRowIdx < nTetrisRowCount; ++nRowIdx)
	{
		int nActualRowIdx = stNextPos.x - GetShapeRowOffset(nShapeID) + nRowIdx;
		for (int nColIdx = 0; nColIdx < nTetrisColCount; ++nColIdx)
		{
			int nActualColIdx = stNextPos.y - GetShapeColOffset(nShapeID) + nColIdx;
			int nBrickID = GET_BRICKID(nActualRowIdx, nActualColIdx);
			if (GetShapeBrickOriginState(nShapeID, nRowIdx, nColIdx)
				&& (nActualRowIdx < 0
					|| nActualRowIdx >= ROW_COUNT
					|| nActualColIdx < 0
					|| nActualColIdx >= COLUMN_COUNT
					|| GetBrickState(nBrickID)))
			{
				return false;
			}
		}
	}

	return true;
}


bool CTetrisGame::GetShapeBrickState(int nShapeID, int nRowIdx, int nColIdx)
{
	return GetShapeBrickOriginState(nShapeID, nRowIdx, nColIdx);
}


void CTetrisGame::UpdateOther(float dt, bool& bUpdateFlag)
{
}


void CTetrisGame::UpdateValidBricksState()
{
	for (int nIndex = 0; nIndex < ROW_COUNT * COLUMN_COUNT; ++nIndex)
	{
		if (GetBrickState(nIndex))
		{
			m_mapUpdateBricks[nIndex] = true;
		}
	}

	UpdateBrickState(0, GetBrickState(0), true);
}


bool CTetrisGame::LoadTetrisData()
{
	if (!GET_BOOLVALUE("TETRIS_RECORD_VALID", false))
	{
		return false;
	}

	return CGameLogic::GetInstance()->GetDataManager()->LoadTetrisData(TETRIS_DATAPATH,
		m_arrBrickState, m_stTetrisData.nShapeID, m_nNextShapeID, m_nCurScore);
}


bool CTetrisGame::SaveTetrisData(bool bForceFlag /*= false*/)
{
	if (m_enGameStage != STAGE_NORMAL
		|| (!bForceFlag && !GET_BOOLVALUE("TETRIS_RECORD_VALID", false)))
	{
		return false;
	}

	return CGameLogic::GetInstance()->GetDataManager()->SaveTetrisData(TETRIS_DATAPATH,
		m_arrBrickState, m_stTetrisData.nShapeID, m_nNextShapeID, m_nCurScore);
}


bool CTetrisGame::RemoveRows()
{
	int nRemoveCount = 0;
	for (int nRowIdx = ROW_COUNT - 1; nRowIdx >= 0; )
	{
		if (!__CheckOneRowFull(nRowIdx))
		{
			--nRowIdx;
			continue;
		}

		__RemoveOneRow(nRowIdx);
		++nRemoveCount;
	}

	if (nRemoveCount > 0)
	{
		// Play effect
		PlayEffect(EFFECT_DELETE);

		AddScore(REMOVE_ONE_ROW_ADD_SCORE * nRemoveCount);
		return true;
	}

	return false;
}


void CTetrisGame::UpdateShape(int nShapeID, const POSITION& stNewPos, bool bSyncFlag /*= false*/)
{
	if (nShapeID < 0 || nShapeID >= TETRIS_SHAPE_COUNT)
	{
		return;
	}

	TMap_BrickState mapUpdateBricks;

	// Clear old shape
	int nTetrisRowCount = TETRIS_SHAPE_ROW_COUNT[m_stTetrisData.nShapeID];
	int nTetrisColCount = TETRIS_SHAPE_COL_COUNT[m_stTetrisData.nShapeID];
	for (int nRowIdx = 0; nRowIdx < nTetrisRowCount; ++nRowIdx)
	{
		int nActualRowIdx = m_stTetrisData.stPos.x - GetShapeRowOffset(m_stTetrisData.nShapeID) + nRowIdx;
		for (int nColIdx = 0; nColIdx < nTetrisColCount; ++nColIdx)
		{
			bool bVisibleFlag = GetShapeBrickOriginState(m_stTetrisData.nShapeID, nRowIdx, nColIdx);
			if (!bVisibleFlag)
			{
				continue;
			}

			int nActualColIdx = m_stTetrisData.stPos.y - GetShapeColOffset(m_stTetrisData.nShapeID) + nColIdx;
			int nBrickID = GET_BRICKID(nActualRowIdx, nActualColIdx);
			mapUpdateBricks[nBrickID] = false;
		}
	}

	// Draw new shape
	nTetrisRowCount = TETRIS_SHAPE_ROW_COUNT[nShapeID];
	nTetrisColCount = TETRIS_SHAPE_COL_COUNT[nShapeID];
	for (int nRowIdx = 0; nRowIdx < nTetrisRowCount; ++nRowIdx)
	{
		int nActualRowIdx = stNewPos.x - GetShapeRowOffset(nShapeID) + nRowIdx;
		for (int nColIdx = 0; nColIdx < nTetrisColCount; ++nColIdx)
		{
			bool bVisibleFlag = GetShapeBrickOriginState(nShapeID, nRowIdx, nColIdx);
			if (!bVisibleFlag)
			{
				continue;
			}

			int nActualColIdx = stNewPos.y - GetShapeColOffset(nShapeID) + nColIdx;
			int nBrickID = GET_BRICKID(nActualRowIdx, nActualColIdx);
			if (!bSyncFlag)
			{
				bVisibleFlag = (bVisibleFlag && GetShapeBrickState(nShapeID, nRowIdx, nColIdx));
				mapUpdateBricks[nBrickID] = bVisibleFlag;
			}
			else
			{
				UpdateBrickState(nBrickID, true);
			}
		}
	}

	if (!mapUpdateBricks.empty())
	{
		SEventContextBrickStateUpdate stUpdateContext;
		stUpdateContext.bSmallBrickFlag = false;
		stUpdateContext.mapUpdateBricks = mapUpdateBricks;
		g_oEventEngine.FireEvent(EVENT_BRICKSTATE_UPDATE, (const char*)&stUpdateContext, sizeof(stUpdateContext));
	}

	// Update data
	m_stTetrisData.nShapeID = nShapeID;
	m_stTetrisData.stPos = stNewPos;
}


void CTetrisGame::__TetrisControlMove(float dt, bool& bUpdateFlag)
{
	if (m_stTetrisData.nDir != DIR_LEFT 
		&& m_stTetrisData.nDir != DIR_RIGHT)
	{
		return;
	}

	m_stTetrisData.nControlInterval += dt;
	if (m_stTetrisData.nControlInterval < CONTROL_MOVE_INTERVAL)
	{
		return;
	}

	m_stTetrisData.nControlInterval = 0;
	POSITION stNextPos = m_stTetrisData.stPos;
	stNextPos.y += ((m_stTetrisData.nDir == DIR_LEFT) ? -1 : 1);

	// Check pos
	if (CheckShapePos(stNextPos, m_stTetrisData.nShapeID))
	{
		// Update pos
		UpdateShape(m_stTetrisData.nShapeID, stNextPos);
		bUpdateFlag = true;
	}
}


void CTetrisGame::__TetrisMoveDown(float dt, bool& bUpdateFlag)
{
	m_stTetrisData.nMoveInterval += dt;
	if (m_stTetrisData.nMoveInterval < __GetTetrisMoveInterval())
	{
		return;
	}

	bUpdateFlag = true;
	m_stTetrisData.nMoveInterval = 0;
	// New pos
	POSITION stNextPos = m_stTetrisData.stPos;
	++stNextPos.x;

	// Check pos
	if (!CheckShapePos(stNextPos, m_stTetrisData.nShapeID))
	{
		OnTetrisMoveFinish();
		return;
	}

	// Update pos
	UpdateShape(m_stTetrisData.nShapeID, stNextPos);
}


void CTetrisGame::RandomShape(bool bUpdateFlag /*= false*/, int nCurShapeID /*= TETRIS_SHAPE_COUNT*/, int nNextShapeID /*= TETRIS_SHAPE_COUNT*/)
{
	m_stTetrisData.nDir = DIR_MAX;
	m_stTetrisData.nMoveInterval = 0;
	m_stTetrisData.nControlInterval = 0;
	m_stTetrisData.bSpeedUpFlag = false;
	m_stTetrisData.nShapeID = ((nCurShapeID == TETRIS_SHAPE_COUNT) ? m_nNextShapeID : nCurShapeID);
	m_stTetrisData.stPos.x = GetShapeRowOffset(m_stTetrisData.nShapeID);
	m_stTetrisData.stPos.y = (COLUMN_COUNT - 1) / 2;
	if (!CheckShapePos(m_stTetrisData.stPos, m_stTetrisData.nShapeID))
	{
		m_enGameStage = STAGE_FAIL;
		return;
	}
	
	// Update shape
	UpdateShape(m_stTetrisData.nShapeID, m_stTetrisData.stPos);

	// Random next shape
	m_nNextShapeID = ((nNextShapeID == TETRIS_SHAPE_COUNT) ? Random(0, GetShapeCount()) : nNextShapeID);
	// Draw shape
	for (int nRowIdx = 0; nRowIdx < SMALL_BRICK_ROW_COUNT; ++nRowIdx)
	{
		for (int nColIdx = 0; nColIdx < SMALL_BRICK_COLUMN_COUNT; ++nColIdx)
		{
			UpdateSmallBrickState(GET_SMALLBRICKID(nRowIdx, nColIdx), GetShapeBrickOriginState(m_nNextShapeID, nRowIdx, nColIdx));
		}
	}
	UpdateSmallBrickState(0, GetSmallBrickState(0), true);
}


int CTetrisGame::GetShapeRowOffset(int nShapeID)
{
	if (nShapeID >= TETRIS_SHAPE_COUNT
		|| nShapeID < 0)
	{
		return 0;
	}

	return (TETRIS_SHAPE_ROW_COUNT[nShapeID] - 1) / 2;
}


int CTetrisGame::GetShapeColOffset(int nShapeID)
{
	if (nShapeID >= TETRIS_SHAPE_COUNT 
		|| nShapeID < 0)
	{
		return 0;
	}

	return (TETRIS_SHAPE_COL_COUNT[nShapeID] - 1) / 2;
}


bool CTetrisGame::GetShapeBrickOriginState(int nShapeID, int nRowIdx, int nColIdx)
{
	if (nShapeID < 0 || nShapeID >= TETRIS_SHAPE_COUNT
		|| nRowIdx < 0 || nRowIdx >= SMALL_BRICK_ROW_COUNT
		|| nColIdx < 0 || nColIdx >= SMALL_BRICK_COLUMN_COUNT)
	{
		return false;
	}

	return TETRIS_SHAPE[nShapeID][nRowIdx][nColIdx];
}


bool CTetrisGame::__CheckOneRowFull(int nRowIdx)
{
	for (int nColIdx = 0; nColIdx < COLUMN_COUNT; ++nColIdx)
	{
		if (!GetBrickState(GET_BRICKID(nRowIdx, nColIdx)))
		{
			return false;
		}
	}

	return true;
}


void CTetrisGame::__RemoveOneRow(int nRowIdx)
{
	for (int nIndex = nRowIdx; nIndex > 0; --nIndex)
	{
		for (int nColIdx = 0; nColIdx < COLUMN_COUNT; ++nColIdx)
		{
			UpdateBrickState(GET_BRICKID(nIndex, nColIdx), GetBrickState(GET_BRICKID(nIndex - 1, nColIdx)));
		}
	}

	for (int nColIdx = 0; nColIdx < COLUMN_COUNT; ++nColIdx)
	{
		UpdateBrickState(nColIdx, false);
	}
}


int CTetrisGame::__GetNextShapeID()
{
	int nNextShapeID = m_stTetrisData.nShapeID;
	for (int nIndex = 0; nIndex < _countof(TETRIS_TYPE_START_IDX) - 1; ++nIndex)
	{
		int nStartIdx = TETRIS_TYPE_START_IDX[nIndex];
		int nEndIdx = TETRIS_TYPE_START_IDX[nIndex + 1];
		if (m_stTetrisData.nShapeID >= nStartIdx && m_stTetrisData.nShapeID < nEndIdx)
		{
			if (++nNextShapeID >= nEndIdx)
			{
				nNextShapeID = nStartIdx;
			}
		}
	}

	return nNextShapeID;
}


int CTetrisGame::__GetTetrisMoveInterval()
{
	return (m_stTetrisData.bSpeedUpFlag ? 10 : TETRIS_MOVE_INTERVAL - 40 * m_nSpeed);
}
