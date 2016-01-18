#pragma once
#include "SceneBase.h"

class CRacingGame : public CSceneBase
{
public:
	CRacingGame(CGameScene* pGameScene);
	~CRacingGame();

	//---------------------    CSceneBase    ----------------------
	//初始化
	void Init();

	//更新
	void Play(float dt);

	//获取当前Brick状态
	bool GetBrickState(int iRowIndex, int iColIndex);

	//获取游戏类型
	SCENE_INDEX GetSceneType();

	//左
	void OnLeft();

	//右
	void OnRight();

	//上
	void OnUp();

	//下
	void OnDown();

	//Fire
	void OnFire();

	//---------------------    CSceneBase    ----------------------

private:
	//添加新行
	void AddNewLine();
	
	//画赛车，iRoadIdx表示车道索引，iRowIdx表示行索引，bSelf表示是否是自己
	void DrawCar(int iRoadIdx, int iRowIdx, bool bSelf);

	//初始化Brick
	void InitBrick();

	//是否游戏结束
	bool IsGameOver();

	//更新Brick
	void UpdateBricks();

private:
	enum 
	{
		CAR_MAXNUM = 3,		//同时出现的车最大数量

		ROAD_MAXINDEX = 3,	//车道最大索引，从0开始

		CAR_WIDTH = 3,		//车宽度

		CAR_HEIGHT = 4,		//车高度
	};


private:
	int m_iCarPos;								//车当前位置

	int m_iSpeed;								//速度

	int m_iLevel;								//关卡

	bool m_arrCurBrick[ROW_NUM][COLUMN_NUM];	//Brick状态

	int m_iLife;								//剩余生命

	int m_iRoadSignCount;						//路标行数

	bool m_bFirstShow;							//是否是第一次显示

	int m_fWaitTime;							//等待更新间隔
};

