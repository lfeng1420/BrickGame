#include "stdafx.h"
#include "GameManager.h"
#include "OverGame.h"
#include "SelectGame.h"
#include "TankGame.h"
#include "RaceGame.h"
#include "SnakeGame.h"
#include "MatchGame.h"
#include "FroggerGame.h"
#include "PinballGame.h"
#include "TetrisGame.h"
#include "FlappybirdGame.h"
#include "TetrisGameEx.h"
#include "PinballGameEx.h"
#include "AddBrickGame.h"
#include "HitBrickGame.h"


CGameManager::CGameManager() : m_pCurGameObj(nullptr)
{

}

CGameManager::~CGameManager()
{
    __ReleaseAllGameObj();
}


bool CGameManager::Create()
{
	// Subscribe event
	g_oEventEngine.AddHandler(EVENT_GAME_START, this);
	g_oEventEngine.AddHandler(EVENT_FRAME_UPDATE, this);
	g_oEventEngine.AddHandler(EVENT_BUTTON, this);
	g_oEventEngine.AddHandler(EVENT_SAVE_DATA, this);
	g_oEventEngine.AddHandler(EVENT_PLAY_BGM, this);

    // Register game
    m_mapGameObj[GAMEID_OVER] = new COverGame();
    m_mapGameObj[GAMEID_SELECT] = new CSelectGame();
	m_mapGameObj[GAMEID_TANK] = new CTankGame();
	m_mapGameObj[GAMEID_RACE] = new CRaceGame();
	m_mapGameObj[GAMEID_SNAKE] = new CSnakeGame();
	m_mapGameObj[GAMEID_MATCH] = new CMatchGame();
	m_mapGameObj[GAMEID_FROGGER] = new CFroggerGame();
	m_mapGameObj[GAMEID_PINBALL] = new CPinballGame();
	m_mapGameObj[GAMEID_TETRIS] = new CTetrisGame();
	m_mapGameObj[GAMEID_FLAPPYBIRD] = new CFlappybirdGame();
	m_mapGameObj[GAMEID_TETRISEX] = new CTetrisGameEx();
	m_mapGameObj[GAMEID_PINBALLEX] = new CPinballGameEx();
	m_mapGameObj[GAMEID_HITBRICK] = new CHitBrickGame();
	m_mapGameObj[GAMEID_ADDBRICK] = new CAddBrickGame();

    // Check if nullptr exist
    FOREACH_IN_CONST_CONTAINER(TMap_GameObj, m_mapGameObj, itGameObj)
    {
        if (itGameObj->second == nullptr)
        {
            return false;
        }
    }

    return true;
}


void CGameManager::OnEvent(int nEventID, const char* pContext, int nLen)
{
	switch (nEventID)
	{
	case EVENT_GAME_START:
		__OnGameStart(pContext, nLen);
		break;

	case EVENT_FRAME_UPDATE:
		__OnFrameUpdate(pContext, nLen);
		break;

	case EVENT_BUTTON:
		__OnButtonEvent(pContext, nLen);

	default:
		break;
	}
}


bool CGameManager::OnVote(int nEventID, char* pContext, int nLen)
{
	switch (nEventID)
	{
	case EVENT_BUTTON:
		return __OnVoteButton(pContext, nLen);
		break;

	case EVENT_SAVE_DATA:
		return __OnVoteSaveData(pContext, nLen);
		break;

	case EVENT_PLAY_BGM:
		return __OnVotePlayBGM(pContext, nLen);
		break;

	default:
		return true;
		break;
	}
}


void CGameManager::__OnGameStart(const char* pContext, int nLen)
{
    if (nLen != sizeof(SEventContextGameStart))
    {
        return;
    }

    const SEventContextGameStart* pGameStart = (const SEventContextGameStart*)pContext;
	
	// Start game
	CGameBase* pGameObj = __GetGameObj(pGameStart->nGameID);
	if (pGameObj == nullptr)
	{
		return;
	}

	if (m_pCurGameObj != nullptr)
	{
		m_pCurGameObj->End();
	}

	m_pCurGameObj = pGameObj;
	m_pCurGameObj->ImportContext(pGameStart);
	m_pCurGameObj->Start();
}


void CGameManager::__OnFrameUpdate(const char* pContext, int nLen)
{
	if (nLen != sizeof(SEventContextFrameUpdate))
	{
		return;
	}

	const SEventContextFrameUpdate* pFrameUpdate = (const SEventContextFrameUpdate*)pContext;
	if (m_pCurGameObj != nullptr)
	{
		m_pCurGameObj->Update(pFrameUpdate->dt);
	}
}


CGameBase* CGameManager::__GetGameObj(int nGameID)
{
    TMap_GameObj::iterator itGameObj = m_mapGameObj.find(nGameID);
    if (itGameObj == m_mapGameObj.end())
    {
        return nullptr;
    }

    return itGameObj->second;
}


void CGameManager::__ReleaseAllGameObj()
{
    FOREACH_IN_CONTAINER(TMap_GameObj, m_mapGameObj, itGameObj)
    {
        delete itGameObj->second;
    }

    m_mapGameObj.clear();
}

void CGameManager::__OnButtonEvent(const char* pContext, int nLen)
{
	if (nLen != sizeof(SEventContextButton))
	{
		return;
	}

	const SEventContextButton* pButtonEvent = (const SEventContextButton*)pContext;

	if (m_pCurGameObj == nullptr)
	{
		return;
	}

	// Reset event
	if (pButtonEvent->nButtonID == BTNID_RESET)
	{
		m_pCurGameObj->SwitchToOverGame();
		return;
	}

	// Other event
	m_pCurGameObj->OnButtonEvent(pButtonEvent);
}


bool CGameManager::__OnVoteButton(char* pContext, int nLen)
{
	if (m_pCurGameObj == nullptr 
		|| nLen != sizeof(SEventContextButton))
	{
		return false;
	}

	SEventContextButton* pButton = (SEventContextButton*)pContext;
	int nGameID = m_pCurGameObj->GetGameID();

	if (pButton->nButtonID == BTNID_RESET 
		&& nGameID < GAMEID_IDMIN)
	{
		return false;
	}

	if (pButton->nButtonID == BTNID_START
		&& nGameID == GAMEID_OVER)
	{
		pButton->nButtonID = BTNID_FIRE;
	}

	return true;
}


bool CGameManager::__OnVotePlayBGM(char* pContext, int nLen)
{
	if (m_pCurGameObj != nullptr)
	{
		int nGameID = m_pCurGameObj->GetGameID();
		if (nGameID >= GAMEID_IDMIN && nGameID < GAMEID_MAX)
		{
			return false;
		}
	}

	return true;
}


bool CGameManager::__OnVoteSaveData(char* pContext, int nLen)
{
	if (m_pCurGameObj == nullptr
		|| nLen != sizeof(SEventContextSaveData))
	{
		return false;
	}

	SEventContextSaveData* pSaveData = (SEventContextSaveData*)pContext;
	return m_pCurGameObj->SaveData(pSaveData->bForceFlag);
}
