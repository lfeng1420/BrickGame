#pragma once
#include "SceneBase.h"

class CPinballGame : public CSceneBase
{
public:
	CPinballGame(CGameScene* pGameScene);
	~CPinballGame();

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

	//Fire按下
	void OnFireBtnPressed();

	//Fire释放
	void OnFireBtnReleased();

	//---------------------    CSceneBase    ----------------------

private:
	//初始化数据，与Init不同，Init在切换到该场景时调用，InitData在重置时调用
	void InitData();

	//小球移动
	bool BallMove(float dt);

	//挡板移动
	bool GuardMove(float dt);

	//加分
	void AddScore(int iScore);

	//检查是否通过
	bool CheckGamePass();

private:
	enum 
	{
		GUARD_BRICK_COUNT = 4,				//挡板方块数量

		BALL_MOVE_INTERVAL = 100,			//球移动时间间隔

		BRICKS_ROWCOUNT = 5,				//方块行数

		BTN_CHECK_INTERVAL = 50,			//按钮检查时间间隔

		GAMEPASS_ADDCOUNT = 10,				//增加10次

		GAMEPASS_ADDSCORE = 10,				//通过时每次增加的分数

		GAMEPASS_REFRESH_INTERVAL = 200,	//通过显示刷新时间

		BOOM_REFRESH_INTERVAL = 50,			//爆炸效果刷新时间

		BOOM_SHOWCOUNT = 16,				//闪烁显示爆炸效果次数
	};

private:
	int m_iSpeed;											//速度

	int m_iLevel;											//关卡

	int m_iLife;											//剩余生命

	int m_iScore;											//分数

	bool m_arrBricks[ROW_NUM][COLUMN_NUM];					//方块状态

	POSITION m_stBallPos;									//球位置

	POSITION m_stBallDis;									//球每次移动的距离

	int m_iGuardColIdx;										//挡板所在列

	GAME_STATE m_enGameState;								//游戏状态

	float m_fWaitTime;										//移动等待时间

	float m_fBtnCheckTime;									//按钮检查时间

	bool bLeftMoveFlag;										//左移标记

	bool bRightMoveFlag;									//右移标记

	bool m_bImproveSpeedFlag;								//加速标记

	bool m_bShowBoom;										//爆炸显示/隐藏标记（闪烁）

	int m_iShowBoomCount;									//闪烁显示爆炸效果次数

	int m_iAddScoreCount;									//当前分数增加次数
};

