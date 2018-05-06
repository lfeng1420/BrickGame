#pragma once
#include "GameBase.h"

class CSnakeGame : public CGameBase
{
public:
	// Start
	virtual void Start();

	// Update
	// param: dt, millisecond
	virtual void Update(float dt);

	// Get game id
	virtual EnGameID GetGameID();

	// Button event
	virtual void OnButtonEvent(const SEventContextButton* pButtonEvent);

private:
	// Update snake
	void __UpdateSnake(float dt, bool& bUpdateFlag, bool bAllowGameOverFlag = true);

	// Update apple
	void __UpdateApple(float dt, bool& bUpdateFlag);

	// Random apple pos
	void __RandApplePos();

	// Check gameover
	bool __CheckPosValid(const POSITION& stHeaderPos);

	// Generate all bricks state
	void __UpdateAllBricksState();

	//////////////////////////////////////////////////////////////////////////
	// Get snake move interval
	int __GetSnakeMoveInterval();

private:
	typedef list<POSITION>	TList_SnakeNodePos;

	// Apple data
	struct _TAppleData
	{
		POSITION stPos;
		bool bVisibleFlag;
		int nRefreshInterval;
	};

	// Snake data
	struct _TSnakeData
	{
		POSITION stHeaderPos;
		TList_SnakeNodePos lsNodePos;
		int nDir;
		int nMoveInterval;
		bool bSpeedUpFlag;
	};

	enum
	{
		SNAKE_MOVE_INTERVAL = 500,
		APPLE_REFRESH_INTERVAL = 200,
		SNAKE_BODY_LEN = 2,
		APPLE_ADD_SCORE = 10,
	};

private:
	// Apple data
	_TAppleData		m_stAppleData;

	// Snake data
	_TSnakeData		m_stSnakeData;
};