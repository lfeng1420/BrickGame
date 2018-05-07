#pragma once
#include "GameBase.h"

class CAddBrickGame : public CGameBase
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
	// Init bricks
	void __InitBricks();

	// Update bricks
	void __UpdateBricks(float dt, bool& bUpdateFlag);

	// Update bullets
	void __UpdateBullets(float dt, bool& bUpdateFlag);

	// Self move
	void __SelfMove(float dt, bool& bUpdateFlag);

	// Self fire
	void __SelfFire(float dt, bool& bUpdateFlag);

	// Draw self
	void __DrawSelf(int nNewColIdx);

	// Delete row
	void __DeleteRow(int nRowIdx);

	// Update game stage
	bool __CheckGameOver();

	//////////////////////////////////////////////////////////////////////////
	//
	int __GetBricksMoveInterval();
	
	//
	int __GetBulletMoveInterval();

private:
	enum
	{
		BRICKS_MOVE_INTERVAL = 2500,
		CONTROL_MOVE_INTERVAL = 115,
		CONTROL_FIRE_INTERVAL = 100,
		BULLET_MOVE_INTERVAL = 70,
		SELF_ROW_INDEX = (ROW_COUNT - 2),
		DELETE_ROW_ADD_SCORE = 10,
		PASS_HIT_BRICK_COUNT = 200,
	};

	typedef list<POSITION>	TList_BulletPos;

	struct _TBulletData
	{
		TList_BulletPos lsPos;
		int nMoveInterval;
	};

	struct _TSelfData
	{
		int nFireInterval;
		int nMoveInterval;
		int nColIdx;
		int nHitBrickCount;
		int nDir;
		bool bFireFlag;
	};

	struct _TBricksData
	{
		int nMoveInterval;
	};

private:
	// Bullet data
	_TBulletData	m_stBulletData;

	// Self data
	_TSelfData		m_stSelfData;

	// Bricks data
	_TBricksData	m_stBricksData;
};