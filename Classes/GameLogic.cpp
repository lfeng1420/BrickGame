#include "stdafx.h"
#include "GameLogic.h"


CGameLogic* CGameLogic::GetInstance()
{
    // Singleton
    static CGameLogic* s_pInstance = nullptr;
    if (s_pInstance == nullptr)
    {
        s_pInstance = new CGameLogic();
        if (!s_pInstance->Create())
        {
            delete s_pInstance;
            s_pInstance = nullptr;
        }
    }

    return s_pInstance;
}


bool CGameLogic::Create()
{
	if (!m_oGameManager.Create())
	{
		return false;
	}

	if (!m_oDataManager.Create())
	{
		return false;
	}

	if (!m_oTimerManager.Create())
	{
		return false;
	}

	return true;
}


CGameManager* CGameLogic::GetGameManager()
{
    return &m_oGameManager;
}


CDataManager* CGameLogic::GetDataManager()
{
    return &m_oDataManager;
}


CTimerManager* CGameLogic::GetTimerManager()
{
	return &m_oTimerManager;
}
