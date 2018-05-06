#include "stdafx.h"
#include "SelectGame.h"


static const int GAME_LIFE[GAMEID_MAX] = 
{
	0,	// Over
	0,	// Select
	4,	// Tank,
	4,	// Race,
	4,	// Snake,
	4,	// Match,
	4,	// Frogger,
	4,	// Pinball,
	0,	// Tetris,
	4,	// Flappybird,
	0,	// TetrisEx,
	4,	// PinballEx,
	4,	// HitBrick,
	4,	// AddBrick,
};


void CSelectGame::Start()
{
	CGameBase::Start();

	// Init update game interval
	m_nUpdateGameInterval = -1;
	m_nCurInterval = 0;

	// Get game id record
    m_nGameID = GET_INTVALUE("GAME", GAMEID_IDMIN);
	if (m_nGameID < GAMEID_IDMIN 
		|| m_nGameID >= GAMEID_MAX)
	{
		m_nGameID = GAMEID_IDMIN;
	}

	// Show high score
	UpdateGameData(m_nGameID);

	// Update immediately
    m_nFrameIdx = 0;
	Update(PLAY_ANIM_INTERVAL);

	// Play BGM
	PlayBGM();
}


void CSelectGame::Update(float dt)
{
	bool bUpdateFlag = false;
	__UpdateGameID(dt, bUpdateFlag);
	__UpdateGameAnim(dt, bUpdateFlag);

	if (bUpdateFlag)
	{
		// Update all bricks
		UpdateAllBrickState();
	}
}


EnGameID CSelectGame::GetGameID()
{
    return GAMEID_SELECT;
}


void CSelectGame::OnButtonEvent(const SEventContextButton* pButtonEvent)
{
	if (pButtonEvent->nButtonID == BTNID_FIRE)
	{
		m_nUpdateGameInterval = (pButtonEvent->bPressedFlag ? 0 : -1);
		if (pButtonEvent->bPressedFlag)
		{
			m_nUpdateGameInterval = UPDATE_GAME_INTERVAL;
			Update(0);
		}

		return;
	}

	if (!pButtonEvent->bPressedFlag)
	{
		return;
	}

    switch (pButtonEvent->nButtonID)
    {
        case BTNID_RIGHT:
            {
                if(++m_nLevel > LEVEL_MAX)
				{
					m_nLevel = 0;
				}
				UpdateGameData();
            }
            break;

        case BTNID_DOWN:
            {
                if (--m_nSpeed < 0)
				{
					m_nSpeed = SPEED_MAX;
				}
				UpdateGameData();
            }
            break;

        case BTNID_LEFT:
            {
                if (--m_nLevel < 0)
				{
					m_nLevel = LEVEL_MAX;
				}
				UpdateGameData();
            }
            break;

        case BTNID_UP:          
            {
                if (++m_nSpeed > SPEED_MAX)
				{
					m_nSpeed = 0;
				}
				UpdateGameData();
            }
            break;

        case BTNID_START:
            __StartGame();
            break;

        default:
            break;
    }
}


void CSelectGame::__StartGame()
{
	if (m_nGameID > GAMEID_MAX || m_nGameID < GAMEID_IDMIN)
	{
		return;
	}

	// Temp record
	SET_INTVALUE("GAME", m_nGameID);

	// Stop BGM
	PlayBGM(false, false);

	// Play start effect
	PlayEffect(EFFECT_START);

	// Start game
    SEventContextGameStart stContext;
    stContext.nGameID = m_nGameID;
    stContext.nLevel = m_nLevel;
    stContext.nSpeed = m_nSpeed;
	stContext.nScore = 0;
	stContext.nLife = GAME_LIFE[m_nGameID];
    g_oEventEngine.FireEvent(EVENT_GAME_START, (const char*)&stContext, sizeof(stContext));
}


void CSelectGame::__UpdateGameAnim(float dt, bool& bUpdateFlag)
{
	m_nCurInterval += dt;
	if (m_nCurInterval < PLAY_ANIM_INTERVAL)
	{
		return;
	}

	// Reset
	m_nCurInterval = 0;

	// Get anim data
	if (!CGameLogic::GetInstance()->GetDataManager()->GetGameAnimData(m_nGameID, m_arrBrickState, m_nFrameIdx))
	{
		return;
	}

	// Update frame index
	if (++m_nFrameIdx >= GAMEID_ANIM_COUNT)
	{
		m_nFrameIdx = 0;
	}

	// Need update
	bUpdateFlag = true;
}


void CSelectGame::__UpdateGameID(float dt, bool& bUpdateFlag)
{
	if (m_nUpdateGameInterval < 0)
	{
		return;
	}

	m_nUpdateGameInterval += dt;
	if (m_nUpdateGameInterval < UPDATE_GAME_INTERVAL)
	{
		return;
	}

	m_nUpdateGameInterval = 0;
	bUpdateFlag = true;

	// Next game
	if (++m_nGameID >= GAMEID_MAX)
	{
		m_nGameID = GAMEID_IDMIN;
	}

	// Show high score
	UpdateGameData(m_nGameID);

	// Reset frame index
	m_nFrameIdx = 0;
	m_nCurInterval = PLAY_ANIM_INTERVAL;
}
