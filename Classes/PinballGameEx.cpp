#include "stdafx.h"
#include "PinballGameEx.h"


void CPinballGameEx::Start()
{
	CPinballGame::Start();

	// Init
	m_nGenInterval = 0;
	m_bMoveLeftFlag = (Random(0, ROW_COUNT * COLUMN_COUNT) <= (ROW_COUNT * COLUMN_COUNT) / 2);
}


void CPinballGameEx::UpdateOther(float dt, bool& bUpdateFlag)
{
	m_nGenInterval += dt;
	if (m_nGenInterval < __GetGenBricksInterval())
	{
		return;
	}

	m_nGenInterval = 0;
	bUpdateFlag = true;

	int nTotalBrickCount = ROW_COUNT * COLUMN_COUNT;
	int nStartRowIdx = GetBlocksStartRowIdx();
	if (m_bMoveLeftFlag)
	{
		for (int nColIdx = COLUMN_COUNT - 1; nColIdx > 0; --nColIdx)
		{
			for (int nRowIdx = nStartRowIdx; nRowIdx < nStartRowIdx + BLOCKS_ROW_COUNT; ++nRowIdx)
			{
				UpdateBrickState(GET_BRICKID(nRowIdx, nColIdx), GetBrickState(GET_BRICKID(nRowIdx, nColIdx - 1)));
			}
		}
	}
	else
	{
		for (int nColIdx = 0; nColIdx < COLUMN_COUNT - 1; ++nColIdx)
		{
			for (int nRowIdx = nStartRowIdx; nRowIdx < nStartRowIdx + BLOCKS_ROW_COUNT; ++nRowIdx)
			{
				UpdateBrickState(GET_BRICKID(nRowIdx, nColIdx), GetBrickState(GET_BRICKID(nRowIdx, nColIdx + 1)));
			}
		}
	}

	for (int nRowIdx = nStartRowIdx; nRowIdx < nStartRowIdx + BLOCKS_ROW_COUNT; ++nRowIdx)
	{
		UpdateBrickState(GET_BRICKID(nRowIdx, m_bMoveLeftFlag ? 0 : (COLUMN_COUNT - 1)), Random(0, nTotalBrickCount) <= nTotalBrickCount / 2);
	}
}


EnGameID CPinballGameEx::GetGameID()
{
	return GAMEID_PINBALLEX;
}


int CPinballGameEx::__GetGenBricksInterval()
{
	return GEN_BRICKS_INTERVAL - 40 * m_nSpeed;
}
