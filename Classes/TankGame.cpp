#include "stdafx.h"
#include "TankGame.h"

static const POSITION CORNER_POS[] =
{
	{ 1, 1 },                             // Left top
	{ 1, COLUMN_COUNT - 2 },              // Right top
	{ ROW_COUNT - 2, 1 },                 // Left bottom
	{ ROW_COUNT - 2, COLUMN_COUNT - 2 },  // Right bottom
};

static const int TANK_BRICK_COUNT = 6;
static const POSITION TANK_OFFSET[DIR_MAX][TANK_BRICK_COUNT] =
{
	{
		{ -1, -1 }, { -1, 0 },
					{ 0, 0 }, { 0, 1 },
		{ 1, -1 }, { 1, 0 },
	},
	{
		{ -1, -1 },			{ -1, 1 },
		{ 0, -1 }, { 0, 0 }, { 0, 1 },
					{ 1, 0 },
	},
	{
				{ -1, 0 }, { -1, 1 },
		{0, -1}, { 0, 0 },
				{ 1, 0 }, { 1, 1 },
	},
	{
					 {-1, 0},
			{0, -1}, {0, 0 }, {0, 1},
			{1, -1},          {1, 1},
	},
};

static const int TANK_SPECIAL_BRICK_COUNT = 3;
static const POSITION TANK_SPECIAL_OFFSET[DIR_MAX][TANK_SPECIAL_BRICK_COUNT] =
{
	{
		{ -1, 1 }, { 1, 1 }, {0, -1},
	},
	{
		{ 1, -1 }, { 1, 1 }, {-1, 0},
	},
	{
		{ -1, -1 }, { 1, -1 }, {0, 1},
	},
	{
		{ -1, -1 },{ -1, 1 }, {1, 0},
	},
};

// Boss shape
static const POSITION BOSS_SHAPE[] = 
{
	{ -7, -4 },																			{ -7, 4 },
	{ -6, -4 },{ -6, -3 },{ -6, -2 },{ -6, -1 },{ -6, 0 },{ -6, 1 },{ -6, 2 },{ -6, 3 },{ -6, 4 },
			 { -5, -3 },{ -5, -2 },{ -5, -1 },{ -5, 0 },{ -5, 1 },{ -5, 2 },{ -5, 3 },
			 { -4, -3 },		   { -4, -1 },{ -4, 0 },{ -4, 1 },		   { -4, 3 },
			 { -3, -3 },		   { -3, -1 },{ -3, 0 },{ -3, 1 },		   { -3, 3 },
			 { -2, -3 },{ -2, -2 },{ -2, -1 },{ -2, 0 },{ -2, 1 },{ -2, 2 },{ -2, 3 },
	{ -1, -4 },								  { -1, 0 },								{ -1, 4 },
											  { 0, 0 },
};



void CTankGame::Start()
{
	CGameBase::Start();

	// Initialize all bullets
	__InitAllBullets();

	// Initialize all tank data
	__InitAllTanks();

	// Initialize create tank data
	m_stCreateTankData.nCreateInterval = 0;
	m_stCreateTankData.nCreateCount = 0;
	m_stCreateTankData.bCreateFinishFlag = false;

	// Initialize game stage
	m_enGameStage = STAGE_NORMAL;

	// Self tank anim data
	m_stSelfTankAnimData.nInterval = 0;
	m_stSelfTankAnimData.bShowFlag = true;

	// Update now
	__UpdateAllBricksState();
}


void CTankGame::Update(float dt)
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
			AddScoreInPassStage(dt, TANK_KILL_ADD_SCORE);
			__UpdateSelfTankAnim(dt, bUpdateFlag);
			if (bUpdateFlag)
			{
				__DrawSelfTankFlag(true);
			}
			
			return;
		}
		break;

	case STAGE_NORMAL:
		{
			__UpdateSelfTankAnim(dt, bUpdateFlag);
			__UpdateBullet(dt, bUpdateFlag);
			__UpdateTank(dt, bUpdateFlag);
			__CreateTank(dt, bUpdateFlag);
		}
		break;

	case STAGE_MOVETOBOTTOM:
		{
			__UpdateSelfTankAnim(dt, bUpdateFlag);
			__UpdateTank(dt, bUpdateFlag);
		}
		break;

	case STAGE_BOSS:
		{
			__UpdateSelfTankAnim(dt, bUpdateFlag);
			__UpdateBullet(dt, bUpdateFlag);
			__UpdateTank(dt, bUpdateFlag);
			__UpdateBossTank(dt, bUpdateFlag);
		}
		break;

	default:
		return;
		break;
	}

	if (bUpdateFlag)
	{
		// Update stage
		__UpdateGameStage();

		// Generate all bricks state
		__UpdateAllBricksState();
	}
}


EnGameID CTankGame::GetGameID()
{
    return GAMEID_TANK;
}


void CTankGame::OnButtonEvent(const SEventContextButton* pButtonEvent)
{
	if (m_enGameStage != STAGE_NORMAL && m_enGameStage != STAGE_BOSS)
	{
		return;
	}

	_TTankData& stTankData = m_arrTankData[0];

	// Fire button
	if (pButtonEvent->nButtonID == BTNID_FIRE)
	{
		// Set fire interval
		stTankData.nFireInterval = pButtonEvent->bPressedFlag ? __GetTankFireInterval(true) : 0;
		stTankData.nCurFireInterval = stTankData.nFireInterval;
		return;
	}

	// Direction button
	if (pButtonEvent->nButtonID >= BTNID_DIRMAX)
	{
		return;
	}
	int nDir = BTNID_2_DIR[pButtonEvent->nButtonID];
	if (nDir != DIR_LEFT && nDir != DIR_RIGHT && m_enGameStage == STAGE_BOSS)
	{
		return;
	}

	// Update self tank direction
	if (pButtonEvent->bPressedFlag)
	{
		stTankData.nDir = nDir;
		stTankData.nCurMoveInterval = 0;
		stTankData.nMoveInterval = __GetTankMoveInterval(true);
		return;
	}

	if (!pButtonEvent->bPressedFlag && stTankData.nDir == nDir)
	{
		stTankData.nCurMoveInterval = 0;
		stTankData.nMoveInterval = 0;
	}
}


void CTankGame::__InitAllTanks()
{
	_TTankData& stSelfTankData = m_arrTankData[0];
	stSelfTankData.bValidFlag = true;
	stSelfTankData.enCampID = enCamp_Self;
	stSelfTankData.nCurFireInterval = 0;
	stSelfTankData.nCurMoveInterval = 0;
	stSelfTankData.nMoveInterval = 0;
	stSelfTankData.nFireInterval = 0;
	stSelfTankData.nDir = DIR_UP;
	stSelfTankData.stPos.x = (ROW_COUNT - 1) / 2;
	stSelfTankData.stPos.y = (COLUMN_COUNT - 1) / 2;
	stSelfTankData.stDestPos.x = 0;
	stSelfTankData.stDestPos.y = 0;

	for (int nIndex = 1; nIndex < NORMAL_STAGE_TANK_COUNT_MAX; ++nIndex)
	{
		m_arrTankData[nIndex].bValidFlag = false;
	}

	// Boss tank
	m_stBossData.bValidFlag = false;
}


void CTankGame::__InitAllBullets()
{
	if (m_vecBulletData.empty())
	{
		return;
	}

	FOREACH_IN_CONTAINER(TVec_BulletData, m_vecBulletData, itBulletData)
	{
		itBulletData->bValidFlag = false;
	}
}


void CTankGame::__CreateTank(float dt, bool& bUpdateFlag)
{
	if (__GetValidTankCount() >= NORMAL_STAGE_TANK_COUNT_MAX
		|| m_stCreateTankData.nCreateCount >= TANK_TOTAL_COUNT)
	{
		return;
	}

	m_stCreateTankData.nCreateInterval += dt;
    if (m_stCreateTankData.nCreateInterval < __GetCreateTankInterval())
    {
        return;
    }

    // Get one invalid tank data
    int nTankIdx = __GetInvalidTankIdx();
    if (nTankIdx >= NORMAL_STAGE_TANK_COUNT_MAX || nTankIdx < 0)
    {
        return;
    }

	// Initialize tank data
	_TTankData& stTankData = m_arrTankData[nTankIdx];
	stTankData.enCampID = enCamp_Enemy;
	stTankData.nCurMoveInterval = 0;
	stTankData.nCurFireInterval = 0;
	stTankData.nFireInterval = 0;
	stTankData.nMoveInterval = 0;

    // Check all corners
	vector<_TTankData> vecTankData;
    for (int nIndex = 0; nIndex < _countof(CORNER_POS); ++nIndex)
    {
        const POSITION& stPos = CORNER_POS[nIndex];
		stTankData.stPos = stPos;
        if (__GenDestPos(nTankIdx, stTankData))
        {
			vecTankData.push_back(stTankData);
        }
    }

	if (vecTankData.empty())
	{
		return;
	}

	// Random
	std::random_shuffle(vecTankData.begin(), vecTankData.end());
	stTankData = vecTankData[0];
	stTankData.bValidFlag = true;

	// Gengerate fire/move interval
	stTankData.nFireInterval = __GetTankFireInterval(false);
	stTankData.nMoveInterval = __GetTankMoveInterval(false);

	// Reset
	m_stCreateTankData.nCreateInterval = 0;
	// Update count
	if (++m_stCreateTankData.nCreateCount == TANK_TOTAL_COUNT)
	{
		m_stCreateTankData.bCreateFinishFlag = true;
	}

	// Need update
	bUpdateFlag = true;
}


int CTankGame::__GetInvalidTankIdx()
{
    for (int nIndex = 1; nIndex < NORMAL_STAGE_TANK_COUNT_MAX; ++nIndex)
    {
        if (!m_arrTankData[nIndex].bValidFlag)
        {
            return nIndex;
        }
    }

    return NORMAL_STAGE_TANK_COUNT_MAX;
}


bool CTankGame::__CheckTankPosValid(int nTankIdx, const _TTankData& stTankData, int nDir)
{
    for (int nIdx = 0; nIdx < TANK_BRICK_COUNT; ++nIdx)
    {
        const POSITION& stSrcOffset = TANK_OFFSET[nDir][nIdx];
        POSITION stBrickPos = { stTankData.stPos.x + stSrcOffset.x, stTankData.stPos.y + stSrcOffset.y};
		if (stBrickPos.x < 0 || stBrickPos.x >= ROW_COUNT
			|| stBrickPos.y < 0 || stBrickPos.y >= COLUMN_COUNT)
		{
			return false;
		}

        int nHitTankIdx = __CheckTankOverlap(nTankIdx, stTankData, stBrickPos);
        if (nHitTankIdx >= 0 && nHitTankIdx < NORMAL_STAGE_TANK_COUNT_MAX)
        {
            return false;
        }
    }

	// Special check
	if (stTankData.enCampID == enCamp_Enemy)
	{
		for (int nIndex = 0; nIndex < TANK_SPECIAL_BRICK_COUNT; ++nIndex)
		{
			const POSITION& stSrcOffset = TANK_SPECIAL_OFFSET[nDir][nIndex];
			POSITION stBrickPos = { stTankData.stPos.x + stSrcOffset.x, stTankData.stPos.y + stSrcOffset.y };
			if (stBrickPos.x < 0 || stBrickPos.x >= ROW_COUNT
				|| stBrickPos.y < 0 || stBrickPos.y >= COLUMN_COUNT)
			{
				return false;
			}

			int nHitTankIdx = __CheckTankOverlap(nTankIdx, stTankData, stBrickPos);
			if (nHitTankIdx >= 0 && nHitTankIdx < NORMAL_STAGE_TANK_COUNT_MAX)
			{
				return false;
			}
		}
	}

    return true;
}


int CTankGame::__CheckTankOverlap(int nSrcTankIdx, const _TTankData& stSrcTankData, const POSITION& stBrickPos)
{
	for (int nTankIdx = 0; nTankIdx < NORMAL_STAGE_TANK_COUNT_MAX; ++nTankIdx)
	{
		if (nTankIdx == nSrcTankIdx)
		{
			continue;
		}

		const _TTankData& stTankData = m_arrTankData[nTankIdx];
		if (!stTankData.bValidFlag)
		{
			continue;
		}

		// Normal check
		for (int nIndex = 0; nIndex < TANK_BRICK_COUNT; ++nIndex)
		{
			const POSITION& stDestOffset = TANK_OFFSET[stTankData.nDir][nIndex];
			int nPosX = stTankData.stPos.x + stDestOffset.x;
			int nPosY = stTankData.stPos.y + stDestOffset.y;
			if (nPosX == stBrickPos.x && nPosY == stBrickPos.y)
			{
				return nTankIdx;
			}
		}

		// Special check
		if (stSrcTankData.enCampID == enCamp_Enemy)
		{
			for (int nIndex = 0; nIndex < TANK_SPECIAL_BRICK_COUNT; ++nIndex)
			{
				const POSITION& stDestOffset = TANK_SPECIAL_OFFSET[stTankData.nDir][nIndex];
				int nPosX = stTankData.stPos.x + stDestOffset.x;
				int nPosY = stTankData.stPos.y + stDestOffset.y;
				if (nPosX == stBrickPos.x && nPosY == stBrickPos.y)
				{
					return nTankIdx;
				}
			}
		}
	}

	return NORMAL_STAGE_TANK_COUNT_MAX;
}


CTankGame::_TTankData* CTankGame::__GetHitTankData(const POSITION& stBrickPos, _EnCampID enCampID)
{
    for (int nTankIdx = 0; nTankIdx < NORMAL_STAGE_TANK_COUNT_MAX; ++nTankIdx)
    {
        _TTankData& stTankData = m_arrTankData[nTankIdx];
		if (!stTankData.bValidFlag || stTankData.enCampID == enCampID)
		{
			continue;
		}

        for (int nIndex = 0; nIndex < TANK_BRICK_COUNT; ++nIndex)
        {
            const POSITION& stDestOffset = TANK_OFFSET[stTankData.nDir][nIndex];
			int nPosX = stTankData.stPos.x + stDestOffset.x;
			int nPosY = stTankData.stPos.y + stDestOffset.y;
            if (nPosX == stBrickPos.x && nPosY == stBrickPos.y)
            {
                return &stTankData;
            }
        }
    }

	if (m_enGameStage == STAGE_BOSS 
		&& EQUAL_POS(stBrickPos, m_stBossData.stPos))
	{
		return &m_stBossData;
	}

    return nullptr;
}


bool CTankGame::__GenDestPos(int nTankIdx, _TTankData& stTankData)
{
	// Get valid direction
	vector<int> vecValidDirection;
	for (int nIndex = DIR_MIN; nIndex < DIR_MAX; ++nIndex)
	{
		if (__CheckTankPosValid(nTankIdx, stTankData, nIndex))
		{
			vecValidDirection.push_back(nIndex);
		}
	}
	if (vecValidDirection.empty())
	{
		return false;
	}

	// Random direction
	std::random_shuffle(vecValidDirection.begin(), vecValidDirection.end());

	stTankData.stDestPos = stTankData.stPos;
	stTankData.nDir = *(vecValidDirection.begin());
    switch (stTankData.nDir)
    {
        case DIR_RIGHT:
			stTankData.stDestPos.y = Random(stTankData.stPos.y, COLUMN_COUNT - 1);
            break;

        case DIR_LEFT:
			stTankData.stDestPos.y = Random(0, stTankData.stPos.y);
            break;

        case DIR_UP:
			stTankData.stDestPos.x = Random(0, stTankData.stPos.x);
            break;

        case DIR_DOWN:
			stTankData.stDestPos.x = Random(stTankData.stPos.x, ROW_COUNT - 1);
            break;

        default:
			return false;
            break;
    }

	return true;
}


void CTankGame::__UpdateBullet(float dt, bool& bUpdateFlag)
{
	// Update pos
	for (unsigned int nIndex = 0; nIndex < m_vecBulletData.size(); ++nIndex)
    {
		_TBulletData& stBulletData = m_vecBulletData[nIndex];
		if (!stBulletData.bValidFlag)
		{
			continue;
		}

		stBulletData.nInterval += dt;
		if (stBulletData.nInterval < __GetBulletMoveInterval())
		{
			continue;
		}

		// Reset
		stBulletData.nInterval = 0;
		bUpdateFlag = true;

		// Move
		if (!GetNextPos(stBulletData.stPos, stBulletData.nDir))
		{
			// Invalid next pos, remove it
			stBulletData.bValidFlag = false;
			continue;
		}
    }

	// Shoot
	for (unsigned int nIndex = 0; nIndex < m_vecBulletData.size(); ++nIndex)
	{
		_TBulletData& stBulletData = m_vecBulletData[nIndex];
		if (!stBulletData.bValidFlag)
		{
			continue;
		}

		// Check aim
		_TTankData* pTankData = __GetHitTankData(stBulletData.stPos, stBulletData.enCampID);
		if (pTankData != nullptr && __HandleTankDead(pTankData))
		{
			bUpdateFlag = true;
			stBulletData.bValidFlag = false;
			continue;
		}

		// Check hit bullet
		for (unsigned int nIdx = nIndex + 1; nIdx < m_vecBulletData.size(); ++nIdx)
		{
			_TBulletData& stData = m_vecBulletData[nIdx];
			if (!stData.bValidFlag
				|| stData.enCampID == stBulletData.enCampID)
			{
				continue;
			}

			if (EQUAL_POS(stData.stPos, stBulletData.stPos))
			{
				bUpdateFlag = true;
				stData.bValidFlag = false;
				stBulletData.bValidFlag = false;
			}
		}
	}
}


bool CTankGame::__HandleTankDead(_TTankData* pTankData)
{
	if (pTankData->enCampID == enCamp_Enemy)
	{
		// Boss
		if (m_enGameStage == STAGE_BOSS)
		{
			// decrease life
			--m_stBossData.nBossLife;
			return true;
		}
		
		// Not boss, add score
		AddScore(TANK_KILL_ADD_SCORE);
	}
	
	// Tank is dead
	pTankData->bValidFlag = false;
	
	return true;
}


void CTankGame::__UpdateTank(float dt, bool& bUpdateFlag)
{
	for (int nIndex = 0; nIndex < NORMAL_STAGE_TANK_COUNT_MAX; ++nIndex)
	{
		_TTankData& stTankData = m_arrTankData[nIndex];
		if (!stTankData.bValidFlag)
		{
			continue;
		}

		// Move
		bUpdateFlag = __OneTankMove(nIndex, stTankData, dt) || bUpdateFlag;

		// Fire
		bUpdateFlag = __OneTankFire(stTankData, dt) || bUpdateFlag;
	}
}


bool CTankGame::__OneTankMove(int nTankIdx, _TTankData& stTankData, float dt)
{
	if (stTankData.nMoveInterval <= 0)
	{
		return false;
	}

	stTankData.nCurMoveInterval += dt;
	if (stTankData.nCurMoveInterval < __GetTankMoveInterval(stTankData.enCampID == enCamp_Self))
	{
		return false;
	}

	// Reset
	stTankData.nCurMoveInterval = 0;

	// Calc next pos
	POSITION stBackupPos = stTankData.stPos;
	if (GetNextPos(stTankData.stPos, stTankData.nDir)
		&& __CheckTankPosValid(nTankIdx, stTankData, stTankData.nDir))
	{
		return true;
	}

	// Recover pos
	stTankData.stPos = stBackupPos;
	if (stTankData.enCampID == enCamp_Enemy)
	{
		// Generate new dest pos (for enemy only)
		__GenDestPos(nTankIdx, stTankData);
	}

	return true;
}


bool CTankGame::__OneTankFire(_TTankData& stTankData, float dt)
{
	if (stTankData.nFireInterval <= 0)
	{
		return false;
	}

	stTankData.nCurFireInterval += dt;
	if (stTankData.nCurFireInterval < stTankData.nFireInterval)
	{
		return false;
	}

	// Reset
	stTankData.nCurFireInterval = 0;
	stTankData.nFireInterval = __GetTankFireInterval(stTankData.enCampID == enCamp_Self);

	// Direction
	int nDir = stTankData.nDir;
	if (m_enGameStage == STAGE_BOSS)
	{
		nDir = (stTankData.enCampID == enCamp_Self) ? DIR_UP : DIR_DOWN;
	}

	// If boss, the bullet need move one time,
	// else, move two times
	POSITION stBulletPos = stTankData.stPos;
	if (!GetNextPos(stBulletPos, nDir))
	{
		return false;
	}
	if (m_enGameStage != STAGE_BOSS && !GetNextPos(stBulletPos, nDir))
	{
		return false;
	}

	// Create bullet
	FOREACH_IN_CONTAINER(TVec_BulletData, m_vecBulletData, itBulletData)
	{
		if (!itBulletData->bValidFlag)
		{
			itBulletData->stPos = stBulletPos;
			itBulletData->nDir = nDir;
			itBulletData->enCampID = stTankData.enCampID;
			itBulletData->nInterval = 0;
			itBulletData->bValidFlag = true;
			return true;
		}
	}

	_TBulletData stBulletData = { stBulletPos, nDir, stTankData.enCampID, 0, true };
	m_vecBulletData.push_back(stBulletData);
	return true;
}


void CTankGame::__UpdateAllBricksState()
{
	for (int nBrickID = 0; nBrickID < ROW_COUNT * COLUMN_COUNT; ++nBrickID)
	{
		UpdateBrickState(nBrickID, false);
	}

	// Draw tank
	for (int nIndex = 0; nIndex < NORMAL_STAGE_TANK_COUNT_MAX; ++nIndex)
	{
		const _TTankData& stTankData = m_arrTankData[nIndex];
		if (!stTankData.bValidFlag)
		{
			continue;
		}

		int nTankDir = (m_enGameStage == STAGE_BOSS) ? DIR_UP : stTankData.nDir;
		const POSITION& stPos = stTankData.stPos;
		for (int nIdx = 0; nIdx < TANK_BRICK_COUNT; ++nIdx)
		{
			const POSITION& stOffset = TANK_OFFSET[nTankDir][nIdx];
			int nBrickID = GET_BRICKID(stPos.x + stOffset.x, stPos.y + stOffset.y);
			UpdateBrickState(nBrickID, true);
		}
	}

	// Draw self tank flag
	__DrawSelfTankFlag(true);

	// Draw bullet
	FOREACH_IN_CONST_CONTAINER(TVec_BulletData, m_vecBulletData, itBulletData)
	{
		if (!itBulletData->bValidFlag)
		{
			continue;
		}

		int nBrickID = GET_BRICKIDBYPOS(itBulletData->stPos);
		UpdateBrickState(nBrickID, true);
	}

	// Draw Boss
	if (m_stBossData.bValidFlag)
	{
		for (int nIdx = 0; nIdx < _countof(BOSS_SHAPE); ++nIdx)
		{
			const POSITION& stOffset = BOSS_SHAPE[nIdx];
			int nBrickID = GET_BRICKID(m_stBossData.stPos.x + stOffset.x, m_stBossData.stPos.y + stOffset.y);
			UpdateBrickState(nBrickID, true);
		}
	}

	// Draw boom
	DrawBoom();

	// Update immediately
	UpdateBrickState(0, GetBrickState(0), true);
}


int CTankGame::__GetValidTankCount()
{
	int nValidCount = 0;
	for (int nIndex = 0; nIndex < NORMAL_STAGE_TANK_COUNT_MAX; ++nIndex)
	{
		if (m_arrTankData[nIndex].bValidFlag)
		{
			++nValidCount;
		}
	}

	return nValidCount;
}


void CTankGame::__UpdateGameStage()
{
	if (m_enGameStage != STAGE_FAIL)
	{
		const _TTankData& stTankData = m_arrTankData[0];
		if (!stTankData.bValidFlag)
		{
			const POSITION& stPos = m_arrTankData[0].stPos;
			POSITION stBoomPos = { stPos.x - 2, stPos.y - 2 };
			GameOverWithBoomAnim(stBoomPos);
			return;
		}
	}

	if (m_stCreateTankData.bCreateFinishFlag)
	{
		if (m_enGameStage == STAGE_NORMAL && __GetValidTankCount() <= 1)
		{
			m_enGameStage = STAGE_MOVETOBOTTOM;
			// Reset self tank fire/move interval
			_TTankData& stTankData = m_arrTankData[0];
			stTankData.nFireInterval = 0;
			stTankData.nCurFireInterval = 0;
			stTankData.nMoveInterval = 0;
			stTankData.nCurMoveInterval = 0;
			// Clear all bullets
			__InitAllBullets();
		}
		else if (m_enGameStage == STAGE_BOSS && m_stBossData.nBossLife <= 0)
		{
			m_enGameStage = STAGE_PASS;
			// Reset
			m_stAddScoreData.nCount = 0;
			m_stAddScoreData.nInterval = 0;
			// Set self tank direction
			m_arrTankData[0].nDir = DIR_UP;
		}
	}

	if (m_enGameStage == STAGE_MOVETOBOTTOM)
	{
		_TTankData& stTankData = m_arrTankData[0];
		if (stTankData.stPos.x < ROW_COUNT - 2)
		{
			stTankData.nDir = DIR_DOWN;
			stTankData.nMoveInterval = __GetTankMoveInterval(true);
			return;
		}

		int nYDis = stTankData.stPos.y - (COLUMN_COUNT - 1) / 2;
		if (nYDis != 0)
		{
			stTankData.nDir = (nYDis < 0) ? DIR_RIGHT : DIR_LEFT;
			stTankData.nMoveInterval = __GetTankMoveInterval(true);
			return;
		}

		stTankData.nDir = DIR_UP;
		stTankData.nMoveInterval = 0;
		stTankData.nFireInterval = 0;

		// Already move to the dest pos, go to the boss stage
		m_enGameStage = STAGE_BOSS;
		// Create boss tank immediately
		__CreateBossTank();
		return;
	}
}


void CTankGame::__CreateBossTank()
{
	m_stBossData.stPos.x = BOSS_SHAPE_ROW_COUNT - 1;
	m_stBossData.stPos.y = (COLUMN_COUNT - 1) / 2;
	m_stBossData.nCurMoveInterval = 0;
	m_stBossData.nCurFireInterval = 0;
	m_stBossData.nDir = DIR_RIGHT;
	m_stBossData.enCampID = enCamp_Enemy;
	m_stBossData.nFireInterval = __GetTankFireInterval(false);
	m_stBossData.nMoveInterval = __GetTankMoveInterval(false);
	m_stBossData.bValidFlag = true;
	m_stBossData.nBossLife = BOSS_TANK_LIFE_COUNT;
}


void CTankGame::__UpdateBossTank(float dt, bool& bUpdateFlag)
{
	// Move
	bUpdateFlag = __BossTankMove(dt) || bUpdateFlag;

	// Fire
	bUpdateFlag = __OneTankFire(m_stBossData, dt) || bUpdateFlag;
}


bool CTankGame::__BossTankMove(float dt)
{
	if (m_stBossData.nMoveInterval <= 0)
	{
		return false;
	}

	m_stBossData.nCurMoveInterval += dt;
	if (m_stBossData.nCurMoveInterval < __GetTankMoveInterval(false))
	{
		return false;
	}

	// Reset
	m_stBossData.nCurMoveInterval = 0;

	// Move
	POSITION stBackupPos = m_stBossData.stPos;
	if (!GetNextPos(m_stBossData.stPos, m_stBossData.nDir)
		|| (m_stBossData.stPos.y > (COLUMN_COUNT - 1) - (BOSS_SHAPE_COLUMN_COUNT - 1) / 2)
		|| (m_stBossData.stPos.y < (BOSS_SHAPE_COLUMN_COUNT - 1) / 2))
	{
		m_stBossData.stPos = stBackupPos;
		m_stBossData.nDir = (m_stBossData.nDir == DIR_LEFT) ? DIR_RIGHT : DIR_LEFT;
	}

	return true;
}


void CTankGame::__UpdateSelfTankAnim(float dt, bool& bUpdateFlag)
{
	m_stSelfTankAnimData.nInterval += dt;
	if (m_stSelfTankAnimData.nInterval < SELF_TANK_ANIM_INTERVAL)
	{
		return;
	}

	// Reset
	m_stSelfTankAnimData.nInterval = 0;
	// Update flag
	m_stSelfTankAnimData.bShowFlag = !m_stSelfTankAnimData.bShowFlag;
	// Need update
	bUpdateFlag = true;
}


void CTankGame::__DrawSelfTankFlag(bool bUpdateFlag /*= false*/)
{
	// Generate pos
	const _TTankData& stSelfTankData = m_arrTankData[0];
	POSITION stPos = stSelfTankData.stPos;
	int nDir = ((m_enGameStage == STAGE_BOSS) ? DIR_UP : stSelfTankData.nDir);
	if (nDir >= DIR_MAX || nDir < DIR_MIN)
	{
		return;
	}
	if (!GetNextPos(stPos, OPPISITE_DIR[nDir]))
	{
		return;
	}

	UpdateBrickState(GET_BRICKIDBYPOS(stPos), m_stSelfTankAnimData.bShowFlag, bUpdateFlag);
}


int CTankGame::__GetCreateTankInterval()
{
    return TANK_CREATE_INTERVAL_BASE - 40 * m_nSpeed;
}


int CTankGame::__GetTankMoveInterval(bool bSelfTankFlag)
{
	return bSelfTankFlag ? (SELF_TANK_MOVE_INTERVAL_BASE - 3 * m_nSpeed) : (TANK_MOVE_INTERVAL_BASE - 70 * m_nSpeed);
}


int CTankGame::__GetTankFireInterval(bool bSelfTankFlag)
{
	return bSelfTankFlag ? (SELF_TANK_FIRE_INTERVAL_BASE - 20 * m_nSpeed) : (Random(1, (TANK_FIRE_INTERVAL_BASE - 300 * m_nSpeed) / 100) * 100);
}


int CTankGame::__GetBulletMoveInterval()
{
	return BULLET_MOVE_INTERVAL - 2 * m_nSpeed;
}
