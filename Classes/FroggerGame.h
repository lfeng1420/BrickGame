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

	//初始化数据、变量等
	void InitData();

private:
	struct RIVER
	{
		bool bLeft;				//是否向左边移动

		int iCurState;			//下一个产生的Brick状态，0表示空

		int iCount;				//当前产生Brick的个数

		int arrMaxCount[2];		//对应状态产生的Brick最大数量

		int arrRefreshTime[2];	//刷新时间

		int iCurTime;			//当前时间
	};

	typedef map<int, RIVER> TMAP_RIVER;

private:
	TMAP_RIVER m_mapRiver;						//河道数据

	bool m_arrBrickState[ROW_NUM][COLUMN_NUM];	//Brick状态

	int m_iLife;								//生命数

	int m_iSpeed;								//当前速度

	int m_iLevel;								//当前等级

	int m_iScore;								//当前分数

	int m_iEndRiverIdx;							//河道最上层所在行数，每一个等级可能增加

	int m_iSelfRowIdx;							//自己所在的行索引

	int m_iSelfColIdx;							//自己所在的列索引
};

