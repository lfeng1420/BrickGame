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
	void UpdateRiver(int iRowIndex, float dt);

	//更新自己
	void UpdateSelf(float dt);

	//初始化数据、变量等
	void InitData();


private:
	enum
	{
		RIVER_TOP_ROWINDEX = 8,		//河道最高所在行

		DEFAULT_REFRESHTIME = 700,	//默认刷新时间，毫秒

		SELF_REFRESHTIME = 60,		//自己刷新的时间，毫秒
	};
	
	struct RIVER 
	{
		bool bLeft;				//是否向左移动

		int iOffset;			//偏移

		int iCurTime;			//当前时间

		int iRefreshTime;		//下一次刷新时间
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

	int m_iSelfCurTime;							//自己下一次更新前累计时间

	bool m_bSelfState;							//当前状态

	int m_iPassedBrickNum;						//通过的Brick数量
};

