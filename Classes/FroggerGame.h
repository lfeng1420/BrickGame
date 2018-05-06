#pragma once
#include "GameBase.h"

class CFroggerGame : public CGameBase
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
	// Init rivers
	void __InitRivers();

	// Update rivers
	void __UpdateRivers(float dt, bool& bUpdateFlag);

	// Update self
	void __UpdateSelf(float dt, bool& bUpdateFlag);

	// Update all bricks state
	void __UpdateAllBricksState();

	// Get river update interval
	int __GetRiverUpdateInterval();

	// Get next pos
	bool __GetNextPos(POSITION& stPos, int nDir);

	// Check game over
	bool __CheckGameOver();

	//////////////////////////////////////////////////////////////////////////

	int __GetStartRowIdx();

	int __GetTopRowIdx();

public:
	enum
	{
		RIVER_UPDATE_INTERVAL = 500,
		SELF_UPDATE_INTERVAL = 100,
		PASS_REQUIRE_COUNT = 5,
		RIVER_COUNT = 5,
		RIVER_COLUMN_COUNT = 20,
		PASS_ONE_ADD_SCORE = 10,
		RIVER_START_ROWIDX = 9,
		UPDATE_RIVER_COUNT_MIN = 1,
		RIVER_TOP_ROWIDX = RIVER_START_ROWIDX - 2,
	};

private:
	struct _TRiverData
	{
		int nRiverIdx;
		int nRowIdx;
		bool bLeftFlag;
		int nOffset;
		int nMoveInterval;
	};

	struct _TSelfData
	{
		POSITION stPos;
		bool bVisibleFlag;
		int nInterval;
		int nPassCount;
	};

	typedef		list<POSITION>		TList_SuccDotPos;

private:
	// Self data
	_TSelfData				m_stSelfData;

	// River data
	_TRiverData				m_arrRiverData[RIVER_COUNT];

	// Succ dot pos list
	TList_SuccDotPos		m_lsSuccDotPos;
};