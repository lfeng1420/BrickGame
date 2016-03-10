#pragma once
#include "SceneBase.h"

class CPuzzleGame : public CSceneBase
{
public:
	CPuzzleGame(CGameScene* pGameScene);
	~CPuzzleGame();

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
	void OnUpBtnPressed();

	//右按下
	void OnUpBtnPressed();

	//Fire按下
	void OnFireBtnPressed();

	//---------------------    CSceneBase    ----------------------

private:
	//初始化数据、变量等
	void InitData();

	//创建方块组
	void CreateBrickGroup();

	//方块组移动
	bool BrickGroupMove();

private:
	enum
	{
		ROAD_COUNT =  3,			//每一次出现三个方块组

		BRICK_GROUP_MAX_WIDTH = 4,	//每组方块的最大宽度

		BRICK_GROUP_MAX_HEIGHT = 5,	//每组方块的最大长度

		BRICK_GROUP_DISTANCE = 5,	//每行方块组之间间距
	};

	struct BRICKGROUP
	{
		int m_iType;			//类型

		int m_iRoadIdx;			//位置索引，也表示所在列位置[(4 + 1) * m_iRoadIdx, (4 + 1) * m_iRoadIdx + 3]

		int m_iRowIdx;			//行位置
	};

private:
	int m_iLife;								//生命数

	int m_iSpeed;								//当前速度

	int m_iLevel;								//当前等级

	int m_iScore;								//当前分数

	BRICKGROUP m_arrBrickGroup[ROAD_COUNT][3];	//方块组序列，最多出现三行9个方块组

	GAME_STATE m_enGameState;					//游戏状态
};