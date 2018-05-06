#include "stdafx.h"
#include "EventEngine.h"

// Global variable
CEventEngine g_oEventEngine;


bool CEventEngine::Create()
{
	return true;
}


void CEventEngine::FireEvent(int nEventID, const char* pContext, int nLen)
{
	// Get handlers
    TMap_EventHandlerList::iterator itHandlerList = m_mapEventHandlerList.find(nEventID);
    if (itHandlerList == m_mapEventHandlerList.end())
    {
        return;
    }

	// Trigger event
    const TList_EventHandler& lsEventHandler = itHandlerList->second;
	TList_EventHandler::const_iterator itHandler = lsEventHandler.cbegin();
	for ( ; itHandler != lsEventHandler.cend(); )
    {
		TList_EventHandler::const_iterator itTempHandler = itHandler++;
        (*itTempHandler)->OnEvent(nEventID, pContext, nLen);
    }
}


bool CEventEngine::FireVote(int nEventID, char* pContext, int nLen)
{
	// Get handlers
	TMap_EventHandlerList::iterator itHandlerList = m_mapEventHandlerList.find(nEventID);
	if (itHandlerList == m_mapEventHandlerList.end())
	{
		return false;
	}

	// Trigger event
	const TList_EventHandler& lsEventHandler = itHandlerList->second;
	FOREACH_IN_CONST_CONTAINER (TList_EventHandler, lsEventHandler, itHandler)
	{
		if (!(*itHandler)->OnVote(nEventID, pContext, nLen))
		{
			return false;
		}
	}

	return true;
}


void CEventEngine::AddHandler(int nEventID, IEventHandler* pHandler)
{
    if (pHandler == nullptr)
    {
        return;
    }

    m_mapEventHandlerList[nEventID].push_back(pHandler);
}


void CEventEngine::RemoveHandler(IEventHandler* pHandler)
{
    if (pHandler == nullptr)
    {
        return;
    }

	TMap_EventHandlerList::iterator itHandlerList = m_mapEventHandlerList.begin();
	for (; itHandlerList != m_mapEventHandlerList.end(); ++itHandlerList)
	{
		TList_EventHandler& lsEventHandler = itHandlerList->second;
		TList_EventHandler::iterator itHandler = lsEventHandler.begin();
		for (; itHandler != lsEventHandler.end(); )
		{
			if (*itHandler == pHandler)
			{
				itHandler = lsEventHandler.erase(itHandler);
				continue;
			}

			++itHandler;
		}
	}
}
