#include "stdafx.h"
#include "TimerManager.h"
#include "ITimerHandler.h"


CTimerManager::CTimerManager()
{
}


CTimerManager::~CTimerManager()
{
}


bool CTimerManager::SetTimer(ITimerHandler* pHandler, int nTimerID, int nInterval, int nCallTimes, const char* szDesc)
{
	if (pHandler == nullptr)
	{
		return false;
	}

	_TTimerData* pTimerData = __GetTimerData(nTimerID, pHandler);
	if (pTimerData != nullptr)
	{
		// Update data
		pTimerData->nInterval = nInterval;
		pTimerData->nCallTimes = nCallTimes;
		pTimerData->strDesc = szDesc;
		return true;
	}

	_TTimerData stTimerData;
	stTimerData.nTimerID = nTimerID;
	stTimerData.nInterval = nInterval;
	stTimerData.pHandler = pHandler;
	stTimerData.strDesc = szDesc;
	stTimerData.nCurInterval = 0;
	stTimerData.nCallTimes = nCallTimes;
	m_lsTimer.push_back(stTimerData);

	return true;
}


void CTimerManager::KillTimer(ITimerHandler* pHandler, int nTimerID)
{
	__RemoveTimerData(nTimerID, pHandler);
}


void CTimerManager::update(float dt)
{
	__UpdateAllTimerData(dt * 1000);
}

//////////////////////////////////////////////////////////////////////////

bool CTimerManager::Create()
{
	// Start update per frame
	Scheduler* pScheduler = Director::getInstance()->getScheduler();
	if (pScheduler == nullptr)
	{
		return false;
	}

	pScheduler->scheduleUpdate(this, 0, false);
	return true;
}


void CTimerManager::Close()
{
	Scheduler* pScheduler = Director::getInstance()->getScheduler();
	if (pScheduler != nullptr)
	{
		pScheduler->unscheduleUpdate(this);
	}
}


CTimerManager::_TTimerData* CTimerManager::__GetTimerData(int nTimerID, ITimerHandler* pHandler)
{
	FOREACH_IN_CONTAINER(TList_Timer, m_lsTimer, itTimer)
	{
		if (itTimer->nTimerID == nTimerID && itTimer->pHandler == pHandler)
		{
			return &(*itTimer);
		}
	}

	return nullptr;
}


void CTimerManager::__RemoveTimerData(int nTimerID, ITimerHandler* pHandler)
{
	FOREACH_IN_CONTAINER(TList_Timer, m_lsTimer, itTimer)
	{
		if (itTimer->nTimerID == nTimerID && itTimer->pHandler == pHandler)
		{
			m_lsTimer.erase(itTimer);
			return;
		}
	}
}


void CTimerManager::__UpdateAllTimerData(int nInterval)
{
	TList_TimerIter itTimer = m_lsTimer.begin();
	for (; itTimer != m_lsTimer.end(); )
	{
		itTimer->nCurInterval += nInterval;
		if (itTimer->nCurInterval < itTimer->nInterval)
		{
			++itTimer;
			continue;
		}

		// Trigger
		itTimer->nCurInterval -= itTimer->nInterval;
		(itTimer->pHandler)->OnTimer(itTimer->nTimerID);

		// Infinity call
		if (itTimer->nCallTimes == INFINITY_CALL)
		{
			++itTimer;
			continue;
		}

		// Decrease call times
		if (--itTimer->nCallTimes <= 0)
		{
			itTimer = m_lsTimer.erase(itTimer);
			continue;
		}

		++itTimer;
	}
}
