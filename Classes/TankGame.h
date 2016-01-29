#pragma once
#include "SceneBase.h"

class CTankGame : public CSceneBase
{
private:
	struct POSITION
	{
		int m_iRowIdx;			//行位置

		int m_iColIdx;			//列位置

		bool operator== (const POSITION& rhs) const
		{
			return this->m_iColIdx == rhs.m_iColIdx && this->m_iRowIdx == rhs.m_iRowIdx;
		}
	};

	//坦克数据
	struct TANK_DATA
	{
		POSITION m_stPos;		//位置

		int m_iDirection;		//移动方向

		int m_iCurStep;			//当前已走步数

		int m_iMaxStep;			//最大步数

		bool m_bNeedReset;		//是否需要重置（步数和方向）

		int m_iCamp;			//阵营

		float m_fFireWaitTime;	//发射等待的时间

		float m_fFireMaxTime;	//本次发射需等待总时间

		bool m_bDead;			//是否死亡
	};

	//子弹数据
	struct BULLET_DATA
	{
		int m_iCamp;			//阵营

		POSITION m_stPos;		//位置

		float m_fMoveTime;		//移动等待时间

		int m_iDirection;			//方向

		bool m_bValid;			//是否有效
	};

public:
	CTankGame(CGameScene* pGameScene);
	~CTankGame();

	//---------------------    CSceneBase    ----------------------
	//初始化
	void Init();

	//更新
	void Play(float dt);

	//获取当前Brick状态
	bool GetBrickState(int iRowIndex, int iColIndex);

	//获取小方块序列中的方块状态
	bool GetSmallBrickState(int iRowIndex, int iColIndex);

	//获取游戏类型
	SCENE_INDEX GetSceneType();

	//左按下
	void OnLeftBtnPressed();

	//左释放
	void OnLeftBtnReleased();

	//右按下
	void OnRightBtnPressed();

	//右释放
	void OnRightBtnReleased();

	//上按下
	void OnUpBtnPressed();

	//上释放
	void OnUpBtnReleased();

	//下按下
	void OnDownPressed();

	//下释放
	void OnDownReleased();

	//Fire按下
	void OnFireBtnPressed();

	//Fire释放
	void OnFireBtnReleased();

	//---------------------    CSceneBase    ----------------------

private:
	//初始化数据，与Init不同，Init在切换到该场景时调用，InitData在重置时调用
	void InitData();

	//创建坦克
	void CreateTank(float dt);

	//画坦克
	bool DrawTank(const POSITION& stPos, int iDirection, const POSITION& stTargetPos);

	//检查位置是否有效
	bool CheckPos(int iTankIdx, const POSITION& stPos);

	//坦克移动
	void TankMove(float dt);

	//更新某个坦克位置
	void UpdateTankPos(int iTankIdx);

	//获取下一个位置
	bool GetNextPos(int iTankIdx, POSITION& stOutPos);

	//获取下一个位置
	bool GetNextPos(const POSITION& stCurPos, int iDirection, POSITION& stOutPos, bool bTankFlag = true);

	//我方坦克移动
	void SelfTankMove(float dt);

	//坦克发射子弹
	void TankFire(float dt);

	//子弹创建
	void CreateBullet(int iTankIdx);

	//子弹移动
	void BulletMove(float dt);

	//子弹
	void BulletShoot();

	//子弹打中的坦克索引，未打中时返回-1
	int GetBulletFireTankIndex(const POSITION& stBulletPos, int iCamp);

private:
	enum
	{
		TANK_MAXNUM = 4,							//坦克最大数量

		BULLET_MAXNUM = 20,						//子弹出现最大数量

		TANK_MOVE_INTERVAL = 800,					//坦克移动时间间隔

		BULLET_MOVE_INTERVAL = 50,					//子弹移动时间间隔

		BULLET_CREATE_MAXTIME = 10000,				//最大发射子弹时间

		TANK_CREATE_TIME = 2 * TANK_MOVE_INTERVAL,	//坦克创建时间间隔

		TANK_MOVE_MAXSTEP = 10,						//坦克移动最大步数

		TANK_SELF_MOVE_INTERVAL = 63,				//我方坦克每次移动等待时间
	};

	//阵营
	enum CAMP
	{
		CAMP_NONE,
		CAMP_A,
		CAMP_B,
	};

	typedef vector<BULLET_DATA> TVEC_BULLET;
	typedef TVEC_BULLET::iterator TVEC_BULLET_ITER;

private:
	TANK_DATA m_arrTank[TANK_MAXNUM];			//坦克数据序列

	BULLET_DATA m_arrBullet[BULLET_MAXNUM];	//子弹数据序列

	TANK_DATA m_stSelfTank;						//坦克自己

	bool m_arrBtnState[DIR_MAX];				//按钮状态

	int m_iSpeed;								//速度

	int m_iLevel;								//关卡

	int m_iLife;								//剩余生命

	int m_iScore;								//分数

	float m_fTankMoveTime;						//坦克移动等待时间

	float m_fTankCreateTime;					//坦克创建时间

	float m_fSelfMoveTime;						//我方坦克移动时间

	int m_iFirstCreateNum;						//第一次创建坦克索引
};

