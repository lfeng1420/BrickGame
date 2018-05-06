#pragma once
#include "IEventHandler.h"

class CGameBase;

class CGameManager : public IEventHandler
{
public:
    CGameManager();
    ~CGameManager();

    // Create
    bool Create();

    // on trigger event
    void OnEvent(int nEventID, const char* pContext, int nLen);

	// On vote
	bool OnVote(int nEventID, char* pContext, int nLen);

private:
    // Start game
    void __OnGameStart(const char* pContext, int nLen);

	// Frame update
	void __OnFrameUpdate(const char* pContext, int nLen);

    // Get game object by game id
    CGameBase* __GetGameObj(int nGameID);

    // Relase all game objects
    void __ReleaseAllGameObj();

	// Button event
	void __OnButtonEvent(const char* pContext, int nLen);

	// Vote button event
	bool __OnVoteButton(char* pContext, int nLen);

	// Vote play BGM
	bool __OnVotePlayBGM(char* pContext, int nLen);
	
	// Vote save data event
	bool __OnVoteSaveData(char* pContext, int nLen);

private:
    typedef map<int, CGameBase*>    TMap_GameObj;
    typedef TMap_GameObj::iterator  TMap_GameObjIter;

private:
    // Game object map
    TMap_GameObj    m_mapGameObj;

    // Current running game object
    CGameBase*      m_pCurGameObj;
};