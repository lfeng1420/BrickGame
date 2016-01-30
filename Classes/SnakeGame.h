#pragma once
#include "SceneBase.h"
class CSnakeGame :
	public CSceneBase
{
public:
	CSnakeGame(CGameScene* pGameScene);
	~CSnakeGame();

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

	//随机苹果位置
	void RandApplePos();

	//蛇更新位置
	void SnakeMove();

	//更新蛇节点位置
	bool UpdateSnakeNode(int iIndex);

private:
	enum
	{
		SNAKE_MOVE_INTERVAL = 1000,			//蛇移动时间间隔
		
		APPLE_REFRESH_INTERVAL = 300,		//苹果闪烁间隔

		SNAKE_DEFAULT_LEN = 3,				//默认蛇身长度

		SNAKE_MAX_LEN = 6,					//蛇身最大长度，会随着等级变化而变化
	};

	struct SNAKE_NODE
	{
		POSITION m_stPos;
		int m_iDirection;
	};

	typedef map<int, SNAKE_NODE> TMAP_SNAKE_NODE;
	typedef TMAP_SNAKE_NODE::iterator TMAP_SNAKE_NODE_ITER;

private:
	TMAP_SNAKE_NODE m_mapSnake;			//蛇节点数据

	TMAP_SNAKE_NODE m_mapCycleNode;		//节点回收序列

	TMAP_SNAKE_NODE m_mapChangeNode;	//方向发生变化的节点记录

	POSITION m_stApplePos;				//苹果所在位置

	int m_iSpeed;						//速度

	int m_iLevel;						//关卡

	int m_iLife;						//剩余生命

	int m_iScore;						//分数

	float m_fSnakeMoveTime;				//蛇移动等待时间

	bool m_bAppleState;					//苹果显示状态

	float m_fAppleShowTime;				//苹果显示等待时间

	GAME_STATE m_enGameState;			//游戏状态
};

