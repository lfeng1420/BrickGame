#pragma once
#include "GameBase.h"

class CFlappybirdGame : public CGameBase
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
	// Pillar move
	void __PillarMove(float dt, bool& bUpdateFlag);

	// Create pillar
	void __CreatePillar(int nIndex, int nColIdx);

	// Bird move
	void __BirdMove(float dt, bool& bUpdateFlag);

	// Update all bricks state
	void __UpdateAllBricksState();

	// Update game state
	void __UpdateGameState();

	//////////////////////////////////////////////////////////////////////////
	// Get bird move interval
	int __GetBirdMoveInterval();

	// Get pillar move interval
	int __GetPillarMoveInterval();

	// Get pillar len max
	int __GetPillarLenMax();

private:
	enum
	{
		PILLAR_COLUMN_DISTANCE = 6,
		PILLAR_COUNT_MAX = 3,
		PILLAR_LEN_MAX = 12,
		BIRD_DOWN_INTERVAL = 700,
		BIRD_DOWN_ACCELERATION = 10,
		BIRD_UP_SPEED = 10,
		PILLAR_MOVE_INTERVAL = 500,
		CONTROL_INTERVAL = 50,
		PILLAR_PASS_ADD_SCORE = 10,
		UPDATE_SPEED_NEED_PILLAR_COUNT = 30,
		BIRD_COLUMN_IDX = (COLUMN_COUNT - 1) / 2,
	};

	
	struct _TPillarData
	{
		int nColIdx;
		int nLen;
	};

	struct _TPillarsData
	{
		_TPillarData arrPillarData[PILLAR_COUNT_MAX];
		int nInterval;
	};

	struct _TBirdData
	{
		int nRowIdx;
		int nInterval;
		int nTotalInterval;
		int nUpInterval;
		bool bControlUpFlag;
		int nPassPillarCount;
	};

private:
	// Pillar data
	_TPillarsData	m_stPillarsData;

	// Bird data
	_TBirdData		m_stBirdData;
};