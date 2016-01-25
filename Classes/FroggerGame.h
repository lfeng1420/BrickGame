#pragma once
#include "SceneBase.h"
class CFroggerGame : public CSceneBase
{
public:
	CFroggerGame(CGameScene* pGameScene);
	~CFroggerGame();

	//---------------------    CSceneBase    ----------------------
	//初始化
	void Init();

	//更新
	void Play(float dt);

	//获取当前Brick状态
	bool GetBrickState(int iRowIndex, int iColIndex);

	//生命数
	bool GetSmallBrickState(int iRowIndex, int iColIndex);

	//获取游戏类型
	SCENE_INDEX GetSceneType();

	//左按下
	void OnLeftBtnPressed();

	//右按下
	void OnRightBtnPressed();

	//上按下
	void OnUpBtnPressed();

	//下按下
	void OnDownPressed();

	//Fire按下
	void OnFireBtnPressed();

	//---------------------    CSceneBase    ----------------------

private:
	//更新所有河道
	void UpdateRivers(float dt);

	//更新指定行的河道
	void UpdateRiver(int iRowIndex);

	//更新自己
	void UpdateSelf(float dt);

	//初始化数据、变量等
	void InitData();

	//更新游戏状态
	void UpdateGameState();

	//设置爆炸
	void SetBoom(float dt);

private:
	enum
	{
		RIVER_ROWTOP_INDEX = 8,		//河道最高所在行

		DEFAULT_REFRESHTIME = 700,	//默认刷新时间，毫秒

		SELF_REFRESHTIME = 60,		//自己刷新的时间，毫秒

		RIVER_COUNT = 5,			//河道数量

		GAMEPASS_COUNT = 5,			//达到该数量时游戏进入下一关

		BOOM_SHOWCOUNT = 10,		//闪烁显示爆炸效果次数

		BOOM_REFRESHTIME = 60,		//爆炸闪烁间隔

		GAMEPASS_WAITTIME = 1000,	//通过后等待的时间
	};
	
	struct RIVER 
	{
		bool bLeft;					//是否向左移动

		int iOffset;				//偏移

		bool arrDefaultState[20];	//默认状态
	};

	typedef map<int, RIVER> TMAP_RIVER;

private:
	TMAP_RIVER m_mapRiverData;					//河道数据

	bool m_arrBrickState[ROW_NUM][COLUMN_NUM];	//Brick状态

	int m_iLife;								//生命数

	int m_iSpeed;								//当前速度

	int m_iLevel;								//当前等级

	int m_iScore;								//当前分数

	int m_iSelfRowIdx;							//自己所在的行索引

	int m_iSelfColIdx;							//自己所在的列索引

	float m_fSelfCurTime;						//自己下一次更新前累计时间

	float m_fRiverCurTime;						//河道当前时间

	int m_iPassCount;							//通过的青蛙数量

	bool m_bSelfState;							//当前状态

	GAME_STATE m_enGameState;					//游戏状态

	int m_iShowBoomCount;						//闪烁显示爆炸效果次数

	float m_fBoomCurTime;						//爆炸当前时间

	float m_fPassCurTime;						//通过后当前时间
};

