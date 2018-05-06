#pragma once
#include "DataManager.h"
#include "GameManager.h"
#include "TimerManager.h"

class CGameLogic
{
public:
    // Get instance
    static CGameLogic* GetInstance();

	// Create module
	bool Create();

    // Get event engine
    CGameManager* GetGameManager();

    // Get data manager
    CDataManager* GetDataManager();

	// Get timer manager
	CTimerManager* GetTimerManager();

private:
    // Game manager
	CGameManager	m_oGameManager;

    // Data manager
    CDataManager	m_oDataManager;

	// Timer manager
	CTimerManager	m_oTimerManager;
};