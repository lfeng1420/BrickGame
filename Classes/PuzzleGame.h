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
	void OnLeftBtnPressed();

	//右按下
	void OnRightBtnPressed();

	//Fire按下
	void OnFireBtnPressed();

	//---------------------    CSceneBase    ----------------------

private:
	//初始化数据、变量等
	void InitData();

	//创建方块组
	void CreateBrickGroup();

	//方块组移动
	bool BrickGroupMove(float dt);

	//找空白位置，返回其索引
	int GetBlankIndex();

	//更换形状
	void RandShape();

private:
	enum
	{
		ROAD_COUNT =  3,					//每一次出现三个方块组
		
		BRICK_GROUP_MIN_WIDTH = 2,			//每组方块的最小宽度

		BRICK_GROUP_MAX_WIDTH = 4,			//每组方块的最大宽度

		BRICK_GROUP_MAX_HEIGHT = 4,			//每组方块的最大长度

		BRICK_GROUP_MIN_HEIGHT = 1,			//每组方块的最小长度

		BRICK_GROUP_MOVE_INTERVAL = 300,	//移动时间间隔
	};

	struct BRICKGROUP
	{
		int m_iColCount;		//列数量

		int m_iRowIdx;			//行位置
	};

private:
	int m_iSpeed;								//当前速度

	int m_iLevel;								//当前等级

	int m_iScore;								//当前分数

	int m_iMoveRowCount;						//移动的行数

	BRICKGROUP m_arrBrickGroup[ROAD_COUNT][3];	//方块组序列，最多出现三行9个方块组

	GAME_STATE m_enGameState;					//游戏状态

	bool m_arrBricks[ROW_NUM][COLUMN_NUM];		//所有方块状态

	float m_fBrickMoveTime;						//方块移动等待时间

	int m_iCurShape;							//当前形状

	int m_iNextShape;							//下一个形状

	int m_iCurColIdx;							//当前所在列
};