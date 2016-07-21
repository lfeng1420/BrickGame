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

	//右按下
	void OnRightBtnPressed();

	//上按下
	void OnUpBtnPressed();

	//下按下
	void OnDownPressed();

	//Fire按下
	void OnFireBtnPressed();

	//Fire释放
	void OnFireBtnReleased();

	//游戏状态
	void SaveGameData();

	//---------------------    CSceneBase    ----------------------


private:
	//初始化数据，与Init不同，Init在切换到该场景时调用，InitData在重置时调用
	void InitData();

	//随机苹果位置
	void RandApplePos();

	//蛇更新位置
	void SnakeMove();

	//检查是否游戏结束
	bool CheckGameOver(const POSITION& stHeaderPos);

	//改变方向
	void ChangeDirection(int iDirection);

private:
	enum
	{
		SNAKE_MOVE_INTERVAL = 500,					//蛇移动时间间隔
		
		APPLE_REFRESH_INTERVAL = 200,				//苹果闪烁间隔

		SNAKE_DEFAULT_LEN = 3,						//默认蛇身长度

		SNAKE_MAX_LEN = 18,							//蛇身最大长度，会随着等级变化而变化

		SNAKE_EAT_ADD_SCORE = 10,					//每次吃掉一个苹果加的分数

		BOOM_SHOWCOUNT = 16,						//闪烁显示爆炸效果次数

		GAMEPASS_ADDSCORE = 10,						//通过时增加10

		GAMEPASS_ADDCOUNT = 10,						//增加10次

		GAMEPASS_REFRESH_INTERVAL = 200,			//通过显示刷新时间

		BOOM_REFRESH_INTERVAL = 50,					//爆炸效果刷新时间
	};

	typedef map<int, POSITION> TMAP_SNAKE_NODE;
	typedef TMAP_SNAKE_NODE::iterator TMAP_SNAKE_NODE_ITER;

private:
	TMAP_SNAKE_NODE m_mapSnakeNodes;	//蛇节点数据

	int m_iSnakeDirection;				//蛇方向

	POSITION m_stApplePos;				//苹果所在位置

	int m_iSpeed;						//速度

	int m_iLevel;						//关卡

	int m_iLife;						//剩余生命

	int m_iScore;						//分数

	float m_fWaitRefreshTime;			//等待刷新时间

	bool m_bAppleState;					//苹果显示状态

	float m_fAppleShowTime;				//苹果显示等待时间

	GAME_STATE m_enGameState;			//游戏状态

	bool m_bShowBoom;					//爆炸显示/隐藏标记（闪烁）

	int m_iShowBoomCount;				//闪烁显示爆炸效果次数

	int m_iAddScoreCount;				//当前分数增加次数

	bool m_bImproveSpeed;				//是否加速
};

