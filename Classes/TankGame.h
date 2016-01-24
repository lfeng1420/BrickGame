#pragma once
#include "SceneBase.h"

struct TANK_POS
{
	int m_iColIdx;				//当前所在列

	int m_iRowIdx;				//当前所在行

	bool operator== (const TANK_POS& stPos) const
	{
		return this->m_iRowIdx == stPos.m_iRowIdx && this->m_iRowIdx == stPos.m_iColIdx;
	};

	TANK_POS& operator+= (const TANK_POS& stPos)
	{
		this->m_iRowIdx += stPos.m_iRowIdx;
		this->m_iColIdx += stPos.m_iColIdx;
		return *this;
	};
};

class CTankGame : public CSceneBase
{
private:
	struct TANK_DATA
	{
		bool m_bDead;					//是否死亡

		TANK_POS m_stTankPos;			//所在位置

		int m_iLastStep;				//上一次走的步数

		int m_iCurStep;					//当前走的步数

		int m_iMaxStep;					//最大步数

		int m_iRetryNum;				//重试次数，超过3次重新随机一个方向

		float m_iCurTime;				//射击已等待时间

		float m_iNextFireTime;			//下一次射击的时间

		int m_iDirection;				//当前方向
	};

	enum
	{
		TANK_MAXNUM = 4,					//坦克最大数量

		TANK_REFRESH_TIME = 1000,			//每秒刷新一次

		TANK_FIRE_MAXTIME = 5000,			//射击等待最长时间

		TANK_RETRYMOVE_MAXCOUNT = 3,		//尝试移动的最大次数

		TANK_MOVE_MAXSTEP = ROW_NUM / 2,	//最大移动步数
	};

public:
	CTankGame(CGameScene* pGameScene);
	~CTankGame();

	//---------------------    CSceneBase    ----------------------
	//初始化
	void Init();

	//更新
	void Play(float dt);

	//提供给每次更新单个Brick游戏（当前只有游戏结束）使用，获取当前改变的Brick行列索引
	void GetCurPos(int& iRowIndex, int& iColIndex);

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

	//数量不足时创建坦克
	void CreateTank();

	//检查指定位置是否有效
	bool CheckPosValid(const TANK_POS& stDestPos, int iDirection);

	//检查两个坦克位置是否有重叠
	bool CheckTankOverlap(const TANK_POS& stSrcPos, int iSrcDir, const TANK_POS& stDestPos, int iDestDir);

	//更新坦克序列
	void UpdateTankPos();

private:
	TANK_DATA m_arrTankList[TANK_MAXNUM];		//坦克位置集合

	TANK_POS m_stSelf;							//自己所在位置

	int m_iSpeed;								//速度

	int m_iLevel;								//关卡

	int m_iLife;								//剩余生命

	int m_iScore;								//分数

	int m_iCurTime;								//当前已等待的时间
};

