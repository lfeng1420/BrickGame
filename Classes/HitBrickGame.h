#pragma once
#include "GameBase.h"

class CHitBrickGame : public CGameBase
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

	// Update game stage
	bool __CheckGameOver();

	//////////////////////////////////////////////////////////////////////////
	//
	int __GetBricksMoveInterval();

	//
	int __GetBulletMoveInterval();

	//
	int __GetControlFirenterval();

private:
	enum
	{
		BRICKS_MOVE_INTERVAL = 2000,
		CONTROL_MOVE_INTERVAL = 70,
		CONTROL_FIRE_INTERVAL = 160,
		BULLET_MOVE_INTERVAL = 70,
		SELF_ROW_INDEX = (ROW_COUNT - 2),
		HIT_BRICK_ADD_SCORE = 10,
		PASS_HIT_BRICK_COUNT = 300,
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