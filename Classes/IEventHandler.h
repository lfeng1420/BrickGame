#pragma once

class IEventHandler
{
public:
	// On event
    virtual void OnEvent(int nEventID, const char* pContext, int nLen) = 0;
	// On vote
	virtual bool OnVote(int nEventID, char* pContext, int nLen) = 0;
};
