#include "stdafx.h"
#include "RaceGame.h"


static const POSITION SELF_CAR_OFFSET[] = 
{
			  { 0, 0 },
	{ 1, -1 },{ 1, 0 },{ 1, 1 },
			  { 2, 0 },
	{ 3, -1 },{ 3, 0 },{ 3, 1 },
};

static const POSITION ENEMY_CAR_OFFSET[] =
{
			  { 0, 0 },
	{ 1, -1 },{ 1, 0 },{ 1, 1 },
			  { 2, 0 },
	{ 3, -1 },		   { 3, 1 },
};



void CRaceGame::Start()
{
	CGameBase::Start();

	// Initialize all cars
	__InitCars();
	// Initialize all road signs
	__InitRoadSigns();

	// Game stage
	m_enGameStage = STAGE_NORMAL;

	// Other variables
	m_bSpeedUpFlag = false;
	m_nUpdateInterval = 0;
	m_nAddScoreCount = 0;
}


void CRaceGame::Update(float dt)
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
			AddScoreInPassStage(dt, BEYOND_CAR_ADD_SCORE);
			return;
		}
		break;

	case STAGE_NORMAL:
		{
			__UpdateCarsAndRoadSigns(dt, bUpdateFlag);
			__CreateNewCar(bUpdateFlag);
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


EnGameID CRaceGame::GetGameID()
{
	return GAMEID_RACE;
}


void CRaceGame::OnButtonEvent(const SEventContextButton* pButtonEvent)
{
	if (m_enGameStage != STAGE_NORMAL)
	{
		return;
	}

	bool bPressFlag = pButtonEvent->bPressedFlag;
	int nBtnID = pButtonEvent->nButtonID;
	if (nBtnID == BTNID_UP || nBtnID == BTNID_FIRE)
	{
		m_bSpeedUpFlag = bPressFlag;
		return;
	}

	if (bPressFlag 
		&& (nBtnID == BTNID_LEFT || nBtnID == BTNID_RIGHT))
	{
		int nColIdx = m_nSelfCarColIdx;
		nColIdx += (nBtnID == BTNID_LEFT) ? -CAR_COLUMN_COUNT : CAR_COLUMN_COUNT;
		if (nColIdx < CAR_COLUMN_START_INDEX || nColIdx > CAR_COLUMN_END_INDEX)
		{
			return;
		}

		if (__CheckCarOverlap(nColIdx, false))
		{
			POSITION stPos = { CAR_SELF_ROW_COUNT + 1, nColIdx - CAR_COLUMN_COUNT / 2 };
			GameOverWithBoomAnim(stPos);
			Update(0);
			return;
		}

		m_nSelfCarColIdx = nColIdx;
	}
}


void CRaceGame::__InitCars()
{
	m_nSelfCarColIdx = CAR_COLUMN_START_INDEX + CAR_COLUMN_COUNT;

	for (int nIdx = 0; nIdx < CAR_COUNT_MAX; ++nIdx)
	{
		m_arrCarData[nIdx].bValidFlag = false;
	}
}


void CRaceGame::__CreateNewCar(bool& bUpdateFlag)
{
	if (!__CanCreateCar())
	{
		return;
	}

	vector<int> vecCarIdx(CAR_COUNT_MAX);
	if (!__GetInvalidCarIdx(&vecCarIdx[0], CAR_COUNT_MAX))
	{
		return;
	}

	vector<int> vecColIdx;
	for (int nColIdx = CAR_COLUMN_START_INDEX; nColIdx <= CAR_COLUMN_END_INDEX; nColIdx += CAR_COLUMN_COUNT)
	{
		vecColIdx.push_back(nColIdx);
	}

	if (vecColIdx.empty())
	{
		return;
	}

	// Random
	std::random_shuffle(vecColIdx.begin(), vecColIdx.end());

	// Random car count
	int nCarCount = Random(1, min(vecCarIdx.size(), vecColIdx.size()));
	for (int nIndex = 0; nIndex < nCarCount; ++nIndex)
	{
		int nCarIdx = vecCarIdx[nIndex];
		_TCarData& stCarData = m_arrCarData[nCarIdx];
		stCarData.bValidFlag = true;
		stCarData.stPos.x = 0;
		stCarData.stPos.y = vecColIdx[nIndex];
	}

	// Need update
	bUpdateFlag = true;
}


bool CRaceGame::__CanCreateCar()
{
	for (int nIndex = 0; nIndex < CAR_COUNT_MAX; ++nIndex)
	{
		const _TCarData& stCarData = m_arrCarData[nIndex];
		if (!stCarData.bValidFlag)
		{
			continue;
		}

		if (stCarData.stPos.x < CAR_DISTANCE)
		{
			return false;
		}
	}

	return true;
}


bool CRaceGame::__GetInvalidCarIdx(int* arrCarIdx, int nCount)
{
	if (nCount <= 0)
	{
		return false;
	}

	int nMaxCount = nCount;
	nCount = 0;

	for (int nIndex = 0; nIndex < CAR_COUNT_MAX; ++nIndex)
	{
		if (!m_arrCarData[nIndex].bValidFlag)
		{
			arrCarIdx[nCount] = nIndex;
			if (++nCount >= nMaxCount)
			{
				return true;
			}
		}
	}

	return (nCount > 0);
}


bool CRaceGame::__CheckCarOverlap(int nColIdx, bool bStrictFlag)
{
	for (int nSelfIdx = 0; nSelfIdx < _countof(SELF_CAR_OFFSET); ++nSelfIdx)
	{
		const POSITION& stSelfOffset = SELF_CAR_OFFSET[nSelfIdx];
		int nSelfPosX = CAR_SELF_ROW_COUNT + stSelfOffset.x;
		int nSelfPosY = nColIdx + stSelfOffset.y;
		if (bStrictFlag
			&& (nSelfPosX < 0 || nSelfPosX >= ROW_COUNT
				|| nSelfPosY < CAR_COLUMN_START_INDEX || nSelfPosY > CAR_COLUMN_END_INDEX))
		{
			return true;
		}
		
		for (int nCarIdx = 0; nCarIdx < CAR_COUNT_MAX; ++nCarIdx)
		{
			const _TCarData& stCarData = m_arrCarData[nCarIdx];
			if (!stCarData.bValidFlag)
			{
				continue;
			}

			for (int nEnemyIdx = 0; nEnemyIdx < _countof(ENEMY_CAR_OFFSET); ++nEnemyIdx)
			{
				const POSITION& stEnemyOffset = ENEMY_CAR_OFFSET[nEnemyIdx];
				int nEnemyPosX = stCarData.stPos.x + stEnemyOffset.x;
				int nEnemyPosY = stCarData.stPos.y + stEnemyOffset.y;
				if (nSelfPosY == nEnemyPosY && nSelfPosX == nEnemyPosX)
				{
					return true;
				}
			}
		}
	}

	return false;
}


void CRaceGame::__UpdateCarsAndRoadSigns(float dt, bool& bUpdateFlag)
{
	m_nUpdateInterval += dt;
	if (m_nUpdateInterval < (m_bSpeedUpFlag ? 30 : (CAR_MOVE_INTERVAL - m_nSpeed * 13)))
	{
		return;
	}

	// reset
	m_nUpdateInterval = 0;
	// Need update
	bUpdateFlag = true;

	// Update enemy car pos
	bool bAddScoreFlag = false;
	for (int nIndex = 0; nIndex < CAR_COUNT_MAX; ++nIndex)
	{
		_TCarData& stCarData = m_arrCarData[nIndex];
		if (!stCarData.bValidFlag)
		{
			continue;
		}

		if (++stCarData.stPos.x >= ROW_COUNT)
		{
			bAddScoreFlag = true;
			stCarData.bValidFlag = false;
			continue;
		}
	}

	// Update road sign pos
	TList_RoadSignPos::iterator itPos = m_lsRoadSignPos.begin();
	for ( ; itPos != m_lsRoadSignPos.end(); )
	{
		int& nPosX = *itPos;
		if (++nPosX >= ROW_COUNT)
		{
			itPos = m_lsRoadSignPos.erase(itPos);
			continue;
		}

		++itPos;
	}

	// Create road sign
	int nTopRoadSignPosX = m_lsRoadSignPos.front();
	if (nTopRoadSignPosX > ROAD_SIGN_SEP_COUNT)
	{
		m_lsRoadSignPos.push_front(-(ROAD_SIGN_BRICK_COUNT - 1));
	}

	// Update score
	if (bAddScoreFlag)
	{
		AddScore(BEYOND_CAR_ADD_SCORE);
		// Check pass condition
		if (++m_nAddScoreCount >= FINISH_RACE_SCORE_ADD_COUNT)
		{
			m_enGameStage = STAGE_PASS;
			m_stAddScoreData.nCount = 0;
			m_stAddScoreData.nInterval = 0;
		}
	}

	// Check overlap
	if (__CheckCarOverlap(m_nSelfCarColIdx, false))
	{
		POSITION stPos = { CAR_SELF_ROW_COUNT + 1, m_nSelfCarColIdx - CAR_COLUMN_COUNT / 2 };
		GameOverWithBoomAnim(stPos);
		return;
	}
}


void CRaceGame::__InitRoadSigns()
{
	m_lsRoadSignPos.clear();

	int nStartPosX = ROW_COUNT - (ROAD_SIGN_BRICK_COUNT + ROAD_SIGN_SEP_COUNT);
	int nRoadSignIdx = 0;
	for (; nStartPosX >= 0; nRoadSignIdx += 2)
	{
		m_lsRoadSignPos.push_front(nStartPosX);
		nStartPosX -= (ROAD_SIGN_BRICK_COUNT + ROAD_SIGN_SEP_COUNT);
	}
}


void CRaceGame::__UpdateAllBricksState()
{
	for (int nBrickID = 0; nBrickID < ROW_COUNT * COLUMN_COUNT; ++nBrickID)
	{
		UpdateBrickState(nBrickID, false);
	}

	// Draw road signs
	FOREACH_IN_CONST_CONTAINER (TList_RoadSignPos, m_lsRoadSignPos, itPos)
	{
		for (int nRowOffset = 0; nRowOffset < ROAD_SIGN_BRICK_COUNT; ++nRowOffset)
		{
			UpdateBrickState(GET_BRICKID(*itPos + nRowOffset, COLUMN_COUNT - 1), true);
			UpdateBrickState(GET_BRICKID(*itPos + nRowOffset, 0), true);
		}
	}

	// Draw enemy cars
	for (int nIndex = 0; nIndex < CAR_COUNT_MAX; ++nIndex)
	{
		_TCarData& stCarData = m_arrCarData[nIndex];
		if (!stCarData.bValidFlag)
		{
			continue;
		}

		for (int nEnemyIdx = 0; nEnemyIdx < _countof(ENEMY_CAR_OFFSET); ++nEnemyIdx)
		{
			const POSITION& stEnemyOffset = ENEMY_CAR_OFFSET[nEnemyIdx];
			int nEnemyPosX = stCarData.stPos.x + stEnemyOffset.x;
			int nEnemyPosY = stCarData.stPos.y + stEnemyOffset.y;
			UpdateBrickState(GET_BRICKID(nEnemyPosX, nEnemyPosY), true);
		}
	}

	// Draw boom
	if (!DrawBoom())
	{
		// Draw self
		for (int nSelfIdx = 0; nSelfIdx < _countof(SELF_CAR_OFFSET); ++nSelfIdx)
		{
			const POSITION& stSelfOffset = SELF_CAR_OFFSET[nSelfIdx];
			int nSelfPosX = CAR_SELF_ROW_COUNT + stSelfOffset.x;
			int nSelfPosY = m_nSelfCarColIdx + stSelfOffset.y;
			UpdateBrickState(GET_BRICKID(nSelfPosX, nSelfPosY), true);
		}
	}

	// Update immediately
	UpdateBrickState(0, GetBrickState(0), true);
}
