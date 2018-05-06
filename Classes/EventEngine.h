#pragma once

// Event Engine
// Use this to fire event.
class CEventEngine
{
public:
    // Create
    bool Create();

    // Fire event
    void FireEvent(int nEventID, const char* pContext, int nLen);

	// Fire vote
	bool FireVote(int nEventID, char* pContext, int nLen);

    // add event handler
    void AddHandler(int nEventID, IEventHandler* pHandler);

    // remove event handler
    void RemoveHandler(IEventHandler* pHandler);

private:
    typedef list<IEventHandler*>            TList_EventHandler;
    typedef map<int, TList_EventHandler>    TMap_EventHandlerList;

private:
    // event handler list
    TMap_EventHandlerList	m_mapEventHandlerList;
};


// Global variable
extern CEventEngine g_oEventEngine;
