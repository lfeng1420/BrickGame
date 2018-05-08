#pragma once
#include "GameBase.h"

class CTankGame : public CGameBase
{
    // Start
    void Start();

    // Update
    // param: dt, millisecond
    void Update(float dt);

    // Get game id
    EnGameID GetGameID();

    // Button event
    void OnButtonEvent(const SEventContextButton* pButtonEvent);

private:
    // Camp
	enum _EnCampID
	{
		enCamp_Self,
		enCamp_Enemy,
	};
    
    struct _TTankData
    {
        POSITION stPos;         // Current pos
        POSITION stDestPos;     // Dest pos
		int nDir;				// Direction
        int nCurMoveInterval;   // Current move interval
		int nMoveInterval;		// Move interval
        int nFireInterval;      // Fire interval
        int nCurFireInterval;   // Current fire interval
        _EnCampID enCampID;     // Camp
        bool bValidFlag;        // Valid or not
	};

	struct _TBossTankData : public _TTankData
	{
		int nBossLife;
	};

    struct _TBulletData
    {
        POSITION stPos;         // Current pos
        int nDir;				// Direction
        _EnCampID enCampID;     // Camp
        int nInterval;          // Update interval
		bool bValidFlag;		// Valid or not
	};

    typedef vector<_TBulletData>  TVec_BulletData;
	typedef set<int>			  TSet_InvalidBulletIdx;

    // Config
    enum
    {
        TANK_CREATE_INTERVAL_BASE = 800,
        TANK_FIRE_INTERVAL_BASE = 4000,
		TANK_MOVE_INTERVAL_BASE = 1000,
		SELF_TANK_FIRE_INTERVAL_BASE = 400,
		SELF_TANK_MOVE_INTERVAL_BASE = 55,
		BULLET_MOVE_INTERVAL = 40,
		TANK_KILL_ADD_SCORE = 10,
        NORMAL_STAGE_TANK_COUNT_MAX = 5,
		TANK_TOTAL_COUNT = 30,
		BOSS_SHAPE_ROW_COUNT = 8,
		BOSS_SHAPE_COLUMN_COUNT = 9,
		BOSS_TANK_LIFE_COUNT = 10,
		SELF_TANK_ANIM_INTERVAL = 100,
	};

	struct _TCreateTankData
	{
		int nCreateInterval;
		int nCreateCount;
		bool bCreateFinishFlag;
	};

	struct _TSelfTankAnimData
	{
		int nInterval;
		bool bShowFlag;
	};

private:
	// Init tank data
	void __InitAllTanks();

	// Init all bullets
	void __InitAllBullets();

    // Create tank
    void __CreateTank(float dt, bool& bUpdateFlag);

    // Get invalid tank data index
    int __GetInvalidTankIdx();

    // Check overlap
    bool __CheckTankPosValid(int nTankIdx, const _TTankData& stTankData, int nDir);

	// Check tank overlap
	int __CheckTankOverlap(int nSrcTankIdx, const _TTankData& stSrcTankData, const POSITION& stBrickPos);

    // Check tank hitted or not
	_TTankData* __GetHitTankData(const POSITION& stBrickPos, _EnCampID enCampID);

    // Random dest pos
    bool __GenDestPos(int nTankIdx, _TTankData& stTankData);

    // Update bullet
    void __UpdateBullet(float dt, bool& bUpdateFlag);

    // Handle tank dead
    // If a tank dies, return true
    bool __HandleTankDead(_TTankData* pTankData);

    // Update tank, like pos, fire, etc
    void __UpdateTank(float dt, bool& bUpdateFlag);

	// One tank move
	bool __OneTankMove(int nTankIdx, _TTankData& stTankData, float dt);

	// One tank fire
	bool __OneTankFire(_TTankData& stTankData, float dt);

	// Generate all bricks state
	void __UpdateAllBricksState();

	// Get valid tank count
	int __GetValidTankCount();

	// Update game stage
	void __UpdateGameStage();

	// Create boss tank
	void __CreateBossTank();

	// Update boss tank
	void __UpdateBossTank(float dt, bool& bUpdateFlag);

	// Boss tank move
	bool __BossTankMove(float dt);

	// Update self tank anim
	void __UpdateSelfTankAnim(float dt, bool& bUpdateFlag);

	// Draw self tank flag
	void __DrawSelfTankFlag(bool bUpdateFlag = false);

	//////////////////////////////////////////////////////////////////////////

    // Get create tank interval
    int __GetCreateTankInterval();

	// Get tank move interval
	int __GetTankMoveInterval(bool bSelfTankFlag);

	// Random fire interval
	int __GetTankFireInterval(bool bSelfTankFlag);

	// Get bullet move interval
	int __GetBulletMoveInterval();


private:
    // Tank data
	_TTankData				m_arrTankData[NORMAL_STAGE_TANK_COUNT_MAX];
    
    // Bullet data
    TVec_BulletData			m_vecBulletData;

	// Create tank data
	_TCreateTankData		m_stCreateTankData;

	// Boss data
	_TBossTankData			m_stBossData;

	// Self tank anim data
	_TSelfTankAnimData		m_stSelfTankAnimData;
};
