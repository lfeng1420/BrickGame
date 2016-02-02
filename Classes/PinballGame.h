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

	//---------------------    CSceneBase    ----------------------

private:
	//初始化数据，与Init不同，Init在切换到该场景时调用，InitData在重置时调用
	void InitData();

	//小球移动
	void BallMove(float dt);

	//获取击中的方块索引
	int GetHitBrickIndex(const POSITION& stPos);

private:
	typedef map<int, POSITION> TMAP_BRICK;
	typedef TMAP_BRICK::iterator TMAP_BRICK_ITER;

	enum 
	{
		GUARD_BRICK_COUNT = 4,		//挡板方块数量

		BALL_MOVE_INTERVAL = 150,	//球移动时间间隔
	};

private:
	int m_iSpeed;								//速度

	int m_iLevel;								//关卡

	int m_iLife;								//剩余生命

	int m_iScore;								//分数

	POSITION m_stBall;							//球位置

	POSITION m_stBallDis;						//球每次移动的距离

	POSITION m_stGuard;							//挡板位置

	TMAP_BRICK m_mapBrick;						//各个方块位置

	GAME_STATE m_enGameState;					//游戏状态

	float m_fMoveTime;							//移动等待时间
};

