#pragma once
#include "GameBase.h"

class CPinballGame : public CGameBase
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

public:
	// Other udpate logic
	virtual void UpdateOther(float dt, bool& bUpdateFlag);

	// Get bloks start row index
	int GetBlocksStartRowIdx();

private:
	// Init blocks
	void __InitBlocks();

	// Update ball
	void __UpdateBall(float dt, bool& bUpdateFlag);

	// Update guard
	void __UpdateGuard(float dt, bool& bUpdateFlag);

	// Update guard pos
	void __UpdateGuardPos(const POSITION& stNewPos, bool bClearOldPosFlag = true);

	// Update ball pos
	void __UpdateBallPos(const POSITION& stNewPos, bool bClearOldPosFlag = true);

	// Check game pass
	bool __CheckGamePass();

	//////////////////////////////////////////////////////////////////////////
	// Get ball move interval
	int __GetBallMoveInterval();

	// Get guard move interval
	int __GetGuardMoveInterval();

protected:
	enum
	{
		BALL_MOVE_INTERVAL = 100,
		BLOCKS_ROW_COUNT = 5,
		HIT_BLOCK_ADD_SCORE = 10,
		GUARD_COLUMN_COUNT = 4,
		GUARD_ROW_IDX = ROW_COUNT - 1,
		GUARD_COLUMN_IDX = COLUMN_COUNT / 2 - GUARD_COLUMN_COUNT / 2,
		GUARD_MOVE_INTERVAL = 50,
	};

	struct _TBallData
	{
		POSITION stPos;
		POSITION stIncr;	// Increment
		int nMoveInterval;
	};

	struct _TGuardData
	{
		int nDir;
		int nMoveInterval;
		POSITION stPos;
	};

private:
	// Ball data
	_TBallData		m_stBallData;

	// Guard data
	_TGuardData		m_stGuardData;

	// Speed up flag
	bool			m_bSpeedUpFlag;
};