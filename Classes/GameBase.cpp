#include "stdafx.h"
#include "GameBase.h"


CGameBase::CGameBase() : m_nLevel(0), m_nLife(0), m_nSpeed(0), m_nCurScore(0)
{
}


CGameBase::~CGameBase()
{
}


void CGameBase::Start()
{
	// Clear all bricks state
	INIT_BOOL_ARRAY(m_arrBrickState);
	UpdateAllBrickState();

	// Initialize stage
	m_enGameStage = STAGE_NORMAL;

	// Initialize boom data
	m_stBoomAnimData.bVisibleFlag = false;
	m_stBoomAnimData.nCurFrameCount = 0;
	m_stBoomAnimData.nInterval = 0;
	m_stBoomAnimData.stPos.x = 0;
	m_stBoomAnimData.stPos.y = 0;

	// Initialize add score data
	m_stAddScoreData.nCount = 0;
	m_stAddScoreData.nInterval = 0;
}


void CGameBase::End()
{
}


void CGameBase::Update(float dt)
{

}


EnGameID CGameBase::GetGameID()
{
    return GAMEID_MAX;
}


void CGameBase::OnButtonEvent(const SEventContextButton* pButtonEvent)
{
}


bool CGameBase::SaveData(bool bForceFlag)
{
	return false;
}


void CGameBase::ImportContext(const SEventContextGameStart* pGameStart)
{
    if (pGameStart == nullptr)
    {
        return;
    }

    m_nLevel = pGameStart->nLevel;
    m_nSpeed = pGameStart->nSpeed;
	m_nLife = pGameStart->nLife;
	m_nCurScore = pGameStart->nScore;

	UpdateGameData();
	// Update small bricks
	InitSmallBricks();
}


bool CGameBase::GetBrickState(int nBrickID)
{
    if (nBrickID >= ROW_COUNT * COLUMN_COUNT || nBrickID < 0)
    {
        return false;
    }

    return m_arrBrickState[nBrickID];
}


bool CGameBase::GetSmallBrickState(int nBrickID)
{
    if (nBrickID >= SMALL_BRICK_COUNT || nBrickID < 0)
    {
        return false;
    }

    return m_arrSmallBrickState[nBrickID];
}


void CGameBase::UpdateBrickState(int nBrickID, bool bState, bool bUpdateFlag /*= false*/)
{
	if (nBrickID >= ROW_COUNT * COLUMN_COUNT || nBrickID < 0)
	{
		return;
	}

	if (m_arrBrickState[nBrickID] != bState)
	{
		m_arrBrickState[nBrickID] = bState;
		m_mapUpdateBricks[nBrickID] = bState;
	}

    if (bUpdateFlag && !m_mapUpdateBricks.empty())
    {
        SEventContextBrickStateUpdate stUpdateContext;
        stUpdateContext.bSmallBrickFlag = false;
        stUpdateContext.mapUpdateBricks = m_mapUpdateBricks;
        g_oEventEngine.FireEvent(EVENT_BRICKSTATE_UPDATE, (const char*)&stUpdateContext, sizeof(stUpdateContext));
        m_mapUpdateBricks.clear();
    }
}


void CGameBase::UpdateSmallBrickState(int nBrickID, bool bState, bool bUpdateFlag /*= false*/)
{
	if (nBrickID >= SMALL_BRICK_COUNT || nBrickID < 0)
	{
		return;
	}

	if (m_arrSmallBrickState[nBrickID] != bState)
	{
		m_arrSmallBrickState[nBrickID] = bState;
		m_mapUpdateSmallBricks[nBrickID] = bState;
	}

    if (bUpdateFlag && !m_mapUpdateSmallBricks.empty())
    {
        SEventContextBrickStateUpdate stUpdateContext;
        stUpdateContext.bSmallBrickFlag = true;
        stUpdateContext.mapUpdateBricks = m_mapUpdateSmallBricks;
        g_oEventEngine.FireEvent(EVENT_BRICKSTATE_UPDATE, (const char*)&stUpdateContext, sizeof(stUpdateContext));
        m_mapUpdateSmallBricks.clear();
    }
}


void CGameBase::UpdateAllBrickState()
{
    for (int nBrickID = 0; nBrickID < ROW_COUNT * COLUMN_COUNT; ++nBrickID)
    {
        m_mapUpdateBricks[nBrickID] = m_arrBrickState[nBrickID];
    }

    SEventContextBrickStateUpdate stUpdateContext;
    stUpdateContext.bSmallBrickFlag = false;
    stUpdateContext.mapUpdateBricks = m_mapUpdateBricks;
    g_oEventEngine.FireEvent(EVENT_BRICKSTATE_UPDATE, (const char*)&stUpdateContext, sizeof(stUpdateContext));
    m_mapUpdateBricks.clear();
}


void CGameBase::UpdateAllSmallBrickState()
{
    for (int nBrickID = 0; nBrickID < SMALL_BRICK_COUNT; ++nBrickID)
    {
        m_mapUpdateSmallBricks[nBrickID] = m_arrSmallBrickState[nBrickID];
    }

    SEventContextBrickStateUpdate stUpdateContext;
    stUpdateContext.bSmallBrickFlag = true;
    stUpdateContext.mapUpdateBricks = m_mapUpdateSmallBricks;
    g_oEventEngine.FireEvent(EVENT_BRICKSTATE_UPDATE, (const char*)&stUpdateContext, sizeof(stUpdateContext));
    m_mapUpdateSmallBricks.clear();
}


int CGameBase::Random(int nLow, int nHigh)
{
	bool bSameTime = true;
	static time_t stLastTime = 0;
	time_t tCurTime = time(nullptr);
	if (difftime(tCurTime, stLastTime) > 0)
	{
		srand((unsigned)time(nullptr));
		stLastTime = tCurTime;

		bSameTime = false;
	}

	vector<int> vecNum;
	for (int i = nLow; i < nHigh; ++i)
	{
		vecNum.push_back(i);
	}

	random_shuffle(vecNum.begin(), vecNum.end());

	int iIndex = 0;
	if (bSameTime)
	{
		iIndex = rand() % vecNum.size();
	}
	return vecNum[iIndex];
}


void CGameBase::UpdateGameData(int nGameID /*= GAMEID_MAX*/)
{
	if (nGameID == GAMEID_MAX)
	{
		nGameID = GetGameID();
		if (nGameID < GAMEID_IDMIN || nGameID >= GAMEID_MAX)
		{
			nGameID = GET_INTVALUE("GAME", GAMEID_IDMIN);
		}
	}

	int nMaxScore = CGameLogic::GetInstance()->GetDataManager()->GetHighScore(nGameID);
	SEventContextDataUpdate stDataUpdate = {m_nSpeed, m_nLevel, m_nCurScore, nMaxScore };
	g_oEventEngine.FireEvent(EVENT_DATA_UPDATE, (const char*)&stDataUpdate, sizeof(stDataUpdate));
}


void CGameBase::AddScore(int nAddScore)
{
	//Play effect
	PlayEffect(EFFECT_ADD);

	m_nCurScore = min(m_nCurScore + nAddScore, SCORE_MAX);
	CGameLogic::GetInstance()->GetDataManager()->SetHighScore(GetGameID(), m_nCurScore);
	
	UpdateGameData();
}


void CGameBase::UpdateSpeed(bool bUpdateFlag)
{
	if (++m_nSpeed >= SPEED_MAX)
	{
		m_nSpeed = 0;
		if (++m_nLevel >= LEVEL_MAX)
		{
			m_nLevel = 0;
		}
	}

	if (bUpdateFlag)
	{
		UpdateGameData();
	}
}


void CGameBase::InitSmallBricks()
{
	INIT_BOOL_ARRAY(m_arrSmallBrickState);

	for (int nIndex = 0; nIndex < m_nLife; ++nIndex)
	{
		m_arrSmallBrickState[nIndex] = true;
	}

	UpdateAllSmallBrickState();
}


void CGameBase::AddScoreInPassStage(float dt, int nAddScore)
{
	m_stAddScoreData.nInterval += dt;
	if (m_stAddScoreData.nInterval < GAME_PASS_ADD_SCORE_INTERVAL)
	{
		return;
	}

	// Update count
	m_stAddScoreData.nInterval = 0;
	if (++m_stAddScoreData.nCount >= GAME_PASS_ADD_SCORE_COUNT)
	{
		UpdateSpeed(false);

		// Restart
		Start();

		// Update
		Update(0);
	}

	// Add score
	AddScore(nAddScore);
}


void CGameBase::UpdateBoomAnim(float dt, bool& bUpdateFlag)
{
	m_stBoomAnimData.nInterval += dt;
	if (m_stBoomAnimData.nInterval < BOOM_ANIM_FRAME_INTERVAL)
	{
		return;
	}

	m_stBoomAnimData.nInterval = 0;
	if (++m_stBoomAnimData.nCurFrameCount >= BOOM_ANIM_FRAME_COUNT)
	{
		if (m_nLife > 0)
		{
			if (--m_nLife <= 0)
			{
				// Go to the over game
				SwitchToOverGame();
				return;
			}

			// Update small bricks
			InitSmallBricks();

			// Restart
			Start();
		}
		return;
	}

	m_stBoomAnimData.bVisibleFlag = !m_stBoomAnimData.bVisibleFlag;
	bUpdateFlag = true;
}


void CGameBase::GameOverWithBoomAnim(const POSITION& stPos)
{
	m_enGameStage = STAGE_FAIL;
	
	// Play effect
	PlayEffect(EFFECT_BOOM);

	// Vibrate
	Vibrate();

	// Init boom anim
	m_stBoomAnimData.nCurFrameCount = 0;
	m_stBoomAnimData.nInterval = 0;
	m_stBoomAnimData.bVisibleFlag = true;
	m_stBoomAnimData.stPos = stPos;

	for (int nRowIdx = 0; nRowIdx < BOOM_ROW_COUNT; ++nRowIdx)
	{
		for (int nColIdx = 0; nColIdx < BOOM_COLUMN_COUNT; ++nColIdx)
		{
			int nPosX = m_stBoomAnimData.stPos.x + nRowIdx;
			int nPosY = m_stBoomAnimData.stPos.y + nColIdx;
			if (nPosX >= ROW_COUNT)
			{
				m_stBoomAnimData.stPos.x -= (nPosX - (ROW_COUNT - 1));
			}
			else if (nPosX < 0)
			{
				m_stBoomAnimData.stPos.x += -nPosX;
			}

			if (nPosY >= COLUMN_COUNT)
			{
				m_stBoomAnimData.stPos.y -= (nPosY - (COLUMN_COUNT - 1));
			}
			else if (nPosY < 0)
			{
				m_stBoomAnimData.stPos.y += -nPosY;
			}
		}
	}
}


bool CGameBase::DrawBoom()
{
	if (m_enGameStage != STAGE_FAIL)
	{
		return false;
	}

	for (int nRowIdx = 0; nRowIdx < BOOM_ROW_COUNT; ++nRowIdx)
	{
		for (int nColIdx = 0; nColIdx < BOOM_COLUMN_COUNT; ++nColIdx)
		{
			int nPosX = m_stBoomAnimData.stPos.x + nRowIdx;
			int nPosY = m_stBoomAnimData.stPos.y + nColIdx;
			int nBrickID = GET_BRICKID(nPosX, nPosY);
			UpdateBrickState(nBrickID, m_stBoomAnimData.bVisibleFlag && BOOM_SHAPE[nRowIdx][nColIdx]);
		}
	}

	return true;
}


bool CGameBase::GetNextPos(POSITION& stPos, int nDir)
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
		--stPos.x;
		break;

	case DIR_DOWN:
		++stPos.x;
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


void CGameBase::PlayEffect(const char* szSound)
{
	SEventPlayEffect stEffect = { szSound };
	g_oEventEngine.FireEvent(EVENT_PLAY_EFFECT, (const char*)&stEffect, sizeof(stEffect));
}


void CGameBase::PlayBGM(bool bPlayFlag /*= true*/, bool bLoopFlag /*= true*/)
{
	SEventPlayBGM stBGM = { bPlayFlag, bLoopFlag };
	g_oEventEngine.FireEvent(EVENT_PLAY_BGM, (const char*)&stBGM, sizeof(stBGM));
}


void CGameBase::Vibrate(bool bShortFlag /*= false*/)
{
	SEventVibrate stEvent = { bShortFlag };
	g_oEventEngine.FireEvent(EVENT_VIBRATE, (const char*)&stEvent, sizeof(stEvent));
}


void CGameBase::SwitchToOverGame()
{
	// Save data
	SaveData(false);

	// Fire game over event
	SEventContextGameOver stGameOverContext = {};
	g_oEventEngine.FireEvent(EVENT_GAME_OVER, (const char*)&stGameOverContext, sizeof(stGameOverContext));

	// Start over game
	SEventContextGameStart stStartGame;
	stStartGame.nGameID = GAMEID_OVER;
	stStartGame.nLevel = 0;
	stStartGame.nLife = 0;
	stStartGame.nSpeed = 0;
	stStartGame.nScore = m_nCurScore;
	g_oEventEngine.FireEvent(EVENT_GAME_START, (const char*)&stStartGame, sizeof(stStartGame));
}
