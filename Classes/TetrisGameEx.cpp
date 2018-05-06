#include "stdafx.h"
#include "TetrisGameEx.h"

static const char* TETRISEX_DATAPATH = "./tetrisex.json";


void CTetrisGameEx::Start()
{
	CTetrisGame::Start();

	m_bVisibleFlag = true;
	m_nFlashInterval = 0;
}


int CTetrisGameEx::GetShapeCount()
{
	return TETRIS_SHAPE_COUNT;
}


void CTetrisGameEx::OnTetrisMoveFinish()
{
	if (m_stTetrisData.nShapeID == SHAPEID_BOOM)
	{
		__OnBoomShapeMoveFinish();
	}
	else if (m_stTetrisData.nShapeID == SHAPEID_ADD_BRICK
		|| m_stTetrisData.nShapeID == SHAPEID_REMOVE_BRICK)
	{
		// Special shape can not save
		m_bVisibleFlag = false;
		UpdateShape(m_stTetrisData.nShapeID, m_stTetrisData.stPos);
		m_bVisibleFlag = true;
	}
	else
	{
		// Sync current shape
		UpdateShape(m_stTetrisData.nShapeID, m_stTetrisData.stPos, true);
	}

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


bool CTetrisGameEx::GetShapeBrickState(int nShapeID, int nRowIdx, int nColIdx)
{
	bool bVisibleFlag = GetShapeBrickOriginState(nShapeID, nRowIdx, nColIdx);
	if (nShapeID >= TETRIS_NORMAL_SHAPE_COUNT)
	{
		bVisibleFlag = (bVisibleFlag && m_bVisibleFlag);
	}

	return bVisibleFlag;
}


bool CTetrisGameEx::CheckShapePos(const POSITION& stNextPos, int nShapeID)
{
	if (nShapeID == SHAPEID_DOT)
	{
		if (stNextPos.x < 0 || stNextPos.x >= ROW_COUNT
			|| stNextPos.y < 0 || stNextPos.y >= COLUMN_COUNT)
		{
			return false;
		}

		if (__GetCurFillBrickRowIdx() == m_stTetrisData.stPos.x)
		{
			return false;
		}

		return true;
	}

	if (nShapeID == SHAPEID_BOOM)
	{
		int nTetrisColCount = TETRIS_SHAPE_COL_COUNT[nShapeID];
		int nTetrisRowCount = TETRIS_SHAPE_ROW_COUNT[nShapeID];
		int nLowRowIdx = stNextPos.x - GetShapeRowOffset(nShapeID);
		int nHighRowIdx = nLowRowIdx + nTetrisRowCount - 1;
		int nLowColIdx = stNextPos.y - GetShapeColOffset(nShapeID);
		int nHighColIdx = nLowColIdx + nTetrisColCount - 1;
		if (nLowRowIdx < 0 || nLowRowIdx >= ROW_COUNT
			|| nHighRowIdx < 0 || nHighRowIdx >= ROW_COUNT
			|| nLowColIdx < 0 || nLowColIdx >= COLUMN_COUNT
			|| nHighColIdx < 0 || nHighColIdx >= COLUMN_COUNT)
		{
			return false;
		}

		for (int nColIdx = nLowColIdx; nColIdx <= nHighColIdx; ++nColIdx)
		{
			if (GetBrickState(GET_BRICKID(nHighRowIdx, nColIdx)))
			{
				return false;
			}
		}

		return true;
	}

	return CTetrisGame::CheckShapePos(stNextPos, nShapeID);
}


void CTetrisGameEx::OnButtonEvent(const SEventContextButton* pButtonEvent)
{
	if (m_enGameStage != STAGE_NORMAL)
	{
		return;
	}

	if (pButtonEvent->nButtonID == BTNID_FIRE
		&& pButtonEvent->bPressedFlag)
	{
		if (m_stTetrisData.nShapeID == SHAPEID_ADD_BRICK
			|| m_stTetrisData.nShapeID == SHAPEID_REMOVE_BRICK)
		{
			bool bVisibleFlag = false;
			int nRowIdx = __GetOperateRowIdx();
			if (m_stTetrisData.nShapeID == SHAPEID_ADD_BRICK)
			{
				--nRowIdx;
				bVisibleFlag = true;
			}
			if (nRowIdx < 0 || nRowIdx >= ROW_COUNT)
			{
				return;
			}

			UpdateBrickState(GET_BRICKID(nRowIdx, m_stTetrisData.stPos.y), bVisibleFlag);
			RemoveRows();
			UpdateValidBricksState();
			return;
		}
		else if (m_stTetrisData.nShapeID == SHAPEID_BOOM)
		{
			POSITION stNextPos = { m_stTetrisData.stPos.x + 1, m_stTetrisData.stPos.y};
			bool bCanMoveFlag = (CheckShapePos(m_stTetrisData.stPos, m_stTetrisData.nShapeID) 
				&& CheckShapePos(stNextPos, m_stTetrisData.nShapeID));
			__OnBoomShapeMoveFinish(!bCanMoveFlag);
			// Generate new shape
			RandomShape();
			UpdateValidBricksState();
			return;
		}
	}

	CTetrisGame::OnButtonEvent(pButtonEvent);
}


void CTetrisGameEx::UpdateOther(float dt, bool& bUpdateFlag)
{
	if (m_stTetrisData.nShapeID < TETRIS_NORMAL_SHAPE_COUNT)
	{
		return;
	}

	m_nFlashInterval += dt;
	if (m_nFlashInterval < SHAPE_FLASH_INTERVAL)
	{
		return;
	}

	m_nFlashInterval = 0;
	m_bVisibleFlag = !m_bVisibleFlag;
	UpdateShape(m_stTetrisData.nShapeID, m_stTetrisData.stPos);
	bUpdateFlag = true;
}


void CTetrisGameEx::UpdateValidBricksState()
{
	for (int nIndex = 0; nIndex < ROW_COUNT * COLUMN_COUNT; ++nIndex)
	{
		if (m_stTetrisData.nShapeID == SHAPEID_DOT
			&& nIndex == GET_BRICKIDBYPOS(m_stTetrisData.stPos))
		{
			continue;
		}

		if (GetBrickState(nIndex))
		{
			m_mapUpdateBricks[nIndex] = true;
		}
	}

	UpdateBrickState(0, GetBrickState(0), true);
}


EnGameID CTetrisGameEx::GetGameID()
{
	return GAMEID_TETRISEX;
}


bool CTetrisGameEx::LoadTetrisData()
{
	if (!GET_BOOLVALUE("TETRIS_RECORD_VALID", false))
	{
		return false;
	}

	return CGameLogic::GetInstance()->GetDataManager()->LoadTetrisData(TETRISEX_DATAPATH,
		m_arrBrickState, m_stTetrisData.nShapeID, m_nNextShapeID, m_nCurScore);
}


bool CTetrisGameEx::SaveTetrisData(bool bForceFlag /*= false*/)
{
	if (m_enGameStage != STAGE_NORMAL
		|| (!bForceFlag && !GET_BOOLVALUE("TETRIS_RECORD_VALID", false)))
	{
		return false;
	}

	return CGameLogic::GetInstance()->GetDataManager()->SaveTetrisData(TETRISEX_DATAPATH,
		m_arrBrickState, m_stTetrisData.nShapeID, m_nNextShapeID, m_nCurScore);
}


int CTetrisGameEx::__GetCurFillBrickRowIdx()
{
	int nTetrisRowCount = TETRIS_SHAPE_ROW_COUNT[m_stTetrisData.nShapeID];
	list<int> lsRowIdx;
	for (int nRowIdx = ROW_COUNT - 1; nRowIdx >= m_stTetrisData.stPos.x - GetShapeRowOffset(m_stTetrisData.nShapeID) + nTetrisRowCount; --nRowIdx)
	{
		if (!GetBrickState(GET_BRICKID(nRowIdx, m_stTetrisData.stPos.y)))
		{
			lsRowIdx.push_back(nRowIdx);
		}
	}

	return lsRowIdx.empty() ? m_stTetrisData.stPos.x : lsRowIdx.back();
}


int CTetrisGameEx::__GetOperateRowIdx()
{
	int nTetrisRowCount = TETRIS_SHAPE_ROW_COUNT[m_stTetrisData.nShapeID];
	for (int nRowIdx = m_stTetrisData.stPos.x - GetShapeRowOffset(m_stTetrisData.nShapeID) + nTetrisRowCount; nRowIdx < ROW_COUNT; ++nRowIdx)
	{
		if (GetBrickState(GET_BRICKID(nRowIdx, m_stTetrisData.stPos.y)))
		{
			return nRowIdx;
		}
	}

	return ROW_COUNT;
}


void CTetrisGameEx::__OnBoomShapeMoveFinish(bool bClearBricksFlag /*= true*/)
{
	// Special shape can not save
	m_bVisibleFlag = false;
	UpdateShape(m_stTetrisData.nShapeID, m_stTetrisData.stPos);
	m_bVisibleFlag = true;

	// Play effect
	PlayEffect(EFFECT_BOOM);

	if (!bClearBricksFlag)
	{
		return;
	}

	int nTetrisRowCount = TETRIS_SHAPE_ROW_COUNT[m_stTetrisData.nShapeID];
	int nTetrisColCount = TETRIS_SHAPE_COL_COUNT[m_stTetrisData.nShapeID];
	int nStartRowIdx = m_stTetrisData.stPos.x - GetShapeRowOffset(m_stTetrisData.nShapeID) + nTetrisRowCount;
	int nDelRowCount = (BOOM_REMOVE_ROW_COUNT + (m_stTetrisData.bSpeedUpFlag ? (m_nSpeed / 4 + 1) : 0));
	for (int nRowIdx = nStartRowIdx; nRowIdx < nStartRowIdx + nDelRowCount && nRowIdx < ROW_COUNT; ++nRowIdx)
	{
		for (int nColIdx = 0; nColIdx < nTetrisColCount; ++nColIdx)
		{
			int nAcutalColIdx = m_stTetrisData.stPos.y - GetShapeColOffset(m_stTetrisData.nShapeID) + nColIdx;
			UpdateBrickState(GET_BRICKID(nRowIdx, nAcutalColIdx), false);
		}
	}
}
