#include "stdafx.h"
#include "SnakeGame.h"


void CSnakeGame::Start()
{
	CGameBase::Start();

	// Initialize snake data
	int nCenterX = (ROW_COUNT - 1) / 2;
	int nCenterY = (COLUMN_COUNT - 1) / 2 + SNAKE_BODY_LEN / 2;
	m_stSnakeData.nDir = DIR_RIGHT;
	m_stSnakeData.nMoveInterval = 0;
	m_stSnakeData.stHeaderPos.x = nCenterX;
	m_stSnakeData.stHeaderPos.y = nCenterY;
	m_stSnakeData.bSpeedUpFlag = false;
	m_stSnakeData.lsNodePos.clear();
	for (int nIndex = 0; nIndex < SNAKE_BODY_LEN; ++nIndex)
	{
		POSITION stPos = { nCenterX, --nCenterY };
		m_stSnakeData.lsNodePos.push_back(stPos);
	}

	// Initialize apple data
	m_stAppleData.bVisibleFlag = false;
	m_stAppleData.nRefreshInterval = 0;
	__RandApplePos();

	// Update now
	__UpdateAllBricksState();
}


void CSnakeGame::Update(float dt)
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
			AddScoreInPassStage(dt, APPLE_ADD_SCORE);
			return;
		}
		break;

	case STAGE_NORMAL:
		{
			__UpdateSnake(dt, bUpdateFlag);
			__UpdateApple(dt, bUpdateFlag);
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


EnGameID CSnakeGame::GetGameID()
{
	return GAMEID_SNAKE;
}


void CSnakeGame::OnButtonEvent(const SEventContextButton* pButtonEvent)
{
	if (m_enGameStage != STAGE_NORMAL)
	{
		return;
	}

	if (pButtonEvent->nButtonID == BTNID_FIRE)
	{
		m_stSnakeData.bSpeedUpFlag = pButtonEvent->bPressedFlag;
		return;
	}

	if (pButtonEvent->bPressedFlag 
		&& pButtonEvent->nButtonID < BTNID_DIRMAX)
	{
		int nDir = BTNID_2_DIR[pButtonEvent->nButtonID];
		bool bOppisiteDirFlag = (nDir == OPPISITE_DIR[m_stSnakeData.nDir]);
		if (bOppisiteDirFlag)
		{
			// push header before the first node
			m_stSnakeData.lsNodePos.push_front(m_stSnakeData.stHeaderPos);

			// reverse
			m_stSnakeData.lsNodePos.reverse();

			// update header
			m_stSnakeData.stHeaderPos = m_stSnakeData.lsNodePos.front();
			m_stSnakeData.lsNodePos.pop_front();
		}

		// Update dir
		int nBackupDir = m_stSnakeData.nDir;
		m_stSnakeData.nDir = nDir;

		// Move
		POSITION stOldPos = m_stSnakeData.stHeaderPos;
		bool bUpdateFlag = false;
		__UpdateSnake(__GetSnakeMoveInterval(), bUpdateFlag, !bOppisiteDirFlag);
		if (!EQUAL_POS(stOldPos, m_stSnakeData.stHeaderPos))
		{
			// Update all bricks immediately
			__UpdateAllBricksState();
			return;
		}

		// Recover dir
		m_stSnakeData.nDir = nBackupDir;
	}
}


void CSnakeGame::__UpdateSnake(float dt, bool& bUpdateFlag, bool bAllowGameOverFlag /*= true*/)
{
	m_stSnakeData.nMoveInterval += dt;
	if (m_stSnakeData.nMoveInterval < __GetSnakeMoveInterval())
	{
		return;
	}

	m_stSnakeData.nMoveInterval = 0;
	bUpdateFlag = true;
	// Play effect
	PlayEffect(EFFECT_NEXT);

	// Update header pos
	POSITION stHeaderPos = m_stSnakeData.stHeaderPos;
	GetNextPos(stHeaderPos, m_stSnakeData.nDir);

	// Check game over
	if (!__CheckPosValid(stHeaderPos))
	{
		if (bAllowGameOverFlag)
		{
			GameOverWithBoomAnim(stHeaderPos);
		}
		return;
	}

	// Update other node
	POSITION stNextPos = m_stSnakeData.stHeaderPos;
	FOREACH_IN_CONTAINER(TList_SnakeNodePos, m_stSnakeData.lsNodePos, itNodePos)
	{
		POSITION stTempPos = *itNodePos;
		*itNodePos = stNextPos;
		stNextPos = stTempPos;
	}

	// Update header
	m_stSnakeData.stHeaderPos = stHeaderPos;

	// Check eat apple
	if (EQUAL_POS(stHeaderPos, m_stAppleData.stPos))
	{
		m_stSnakeData.lsNodePos.push_back(stNextPos);
		AddScore(APPLE_ADD_SCORE);
		__RandApplePos();
	}
}


void CSnakeGame::__UpdateApple(float dt, bool& bUpdateFlag)
{
	m_stAppleData.nRefreshInterval += dt;
	if (m_stAppleData.nRefreshInterval < APPLE_REFRESH_INTERVAL)
	{
		return;
	}

	m_stAppleData.nRefreshInterval = 0;
	m_stAppleData.bVisibleFlag = !m_stAppleData.bVisibleFlag;
	bUpdateFlag = true;
}


void CSnakeGame::__RandApplePos()
{
	// Add valid pos
	int nHeadBrickID = GET_BRICKIDBYPOS(m_stSnakeData.stHeaderPos);
	vector<POSITION> vecValidPos;
	for (int nRowIdx = 0; nRowIdx < ROW_COUNT; ++nRowIdx)
	{
		for (int nColIdx = 0; nColIdx < COLUMN_COUNT; ++nColIdx)
		{
			POSITION stPos = { nRowIdx, nColIdx };
			if (nHeadBrickID == GET_BRICKIDBYPOS(stPos) || !__CheckPosValid(stPos))
			{
				continue;
			}

			vecValidPos.push_back(stPos);
		}
	}

	if (vecValidPos.empty())
	{
		return;
	}

	// Random
	std::random_shuffle(vecValidPos.begin(), vecValidPos.end());
	m_stAppleData.nRefreshInterval = 0;
	m_stAppleData.bVisibleFlag = true;
	m_stAppleData.stPos = vecValidPos[0];
}


bool CSnakeGame::__CheckPosValid(const POSITION& stHeaderPos)
{
	if (stHeaderPos.x < 0 || stHeaderPos.x >= ROW_COUNT
		|| stHeaderPos.y < 0 || stHeaderPos.y >= COLUMN_COUNT)
	{
		return false;
	}

	FOREACH_IN_CONST_CONTAINER(TList_SnakeNodePos, m_stSnakeData.lsNodePos, itNode)
	{
		if (EQUAL_POS(stHeaderPos, *itNode))
		{
			return false;
		}
	}

	return true;
}


void CSnakeGame::__UpdateAllBricksState()
{
	for (int nBrickID = 0; nBrickID < ROW_COUNT * COLUMN_COUNT; ++nBrickID)
	{
		UpdateBrickState(nBrickID, false);
	}

	// Draw snake
	UpdateBrickState(GET_BRICKIDBYPOS(m_stSnakeData.stHeaderPos), true);
	FOREACH_IN_CONST_CONTAINER(TList_SnakeNodePos, m_stSnakeData.lsNodePos, itNode)
	{
		UpdateBrickState(GET_BRICKIDBYPOS(*itNode), true);
	}

	// Draw apple
	UpdateBrickState(GET_BRICKIDBYPOS(m_stAppleData.stPos), m_stAppleData.bVisibleFlag);

	// Draw boom
	DrawBoom();

	// Update immediately
	UpdateBrickState(0, GetBrickState(0), true);
}


int CSnakeGame::__GetSnakeMoveInterval()
{
	return (m_stSnakeData.bSpeedUpFlag ? 50 : (SNAKE_MOVE_INTERVAL - m_nSpeed * 30));
}
