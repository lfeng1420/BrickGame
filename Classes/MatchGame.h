#pragma once
#include "SceneBase.h"
class CMatchGame : public CSceneBase
{
public:
	CMatchGame(CGameScene* pGameScene);
	~CMatchGame();

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

	//---------------------    CSceneBase    ----------------------

private:
	//初始化数据，与Init不同，Init在切换到该场景时调用，InitData在重置时调用
	void InitData();

	//目标方块移动，返回值表示是否需要更新界面
	bool DestBricksMove(float dt);

	//我方方块移动
	bool MyBricksMove(float dt);

	//更改方块类型
	void ChangeType(DIRECTION enDirection, bool bPressed);

	//更新我方方块
	void UpdateMyBricks(DIRECTION enDirection);

	//更新游戏状态
	void UpdateGameState();

private:
	enum 
	{
		BRICK_MOVE_INTERVAL = 1000,				//方块移动时间间隔

		BRICK_MATCH_NUM = 3,					//需匹配的方块数量

		BOOM_SHOWCOUNT = 16,					//闪烁显示爆炸效果次数

		MACTHSUCC_ADDSCORE = 10,				//通过时增加10

		GAMEPASS_MATCHCOUNT = 10,				//通过等级需要匹配成功的次数

		GAMEPASS_ADDCOUNT = 10,					//增加10次

		GAMEPASS_REFRESH_INTERVAL = 200,		//通过显示刷新时间

		BOOM_REFRESH_INTERVAL = 50,				//爆炸效果刷新时间

		BRICK_CHANGE_INTERVAL = 60,				//每60毫秒切换一次
	};

private:
	int m_iSpeed;								//速度

	int m_iLevel;								//关卡

	int m_iLife;								//剩余生命

	int m_iScore;								//分数

	int m_iMatchSuccCount;						//匹配成功的计数

	int m_arrDestBrick[3];						//3个目标方块类型

	int m_arrMyBrick[3];						//当前3个方块类型

	int m_arrBoomIndex[3];						//爆炸位置索引

	int m_iDestRowIdx;							//目标方块所在行

	int m_iMyRowIdx;							//我方方块所在行

	float m_fDestBrickMoveTime;					//目标方块移动等待刷新时间

	float m_fWaitTime;							//等待刷新时间/我方方块移动等待时间

	GAME_STATE m_enGameState;					//游戏状态

	bool m_bShowBoom;							//爆炸显示/隐藏标记（闪烁）

	int m_iShowBoomCount;						//闪烁显示爆炸效果次数

	int m_iAddScoreCount;						//当前分数增加次数

	bool m_arrBtnState[4];						//按钮状态

	bool m_bConfirmMatch;						//是否确认匹配
};

