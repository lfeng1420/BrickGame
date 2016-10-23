#pragma once
#include "BGGlobal.h"
#include "SceneBase.h"

class CAddBrickGame : public CSceneBase
{
public:
	CAddBrickGame(CGameScene* pScene, bool bSpecialMode = false);
	~CAddBrickGame();


	////////////////////////////////// CSceneBase ////////////////////////////////////////
	//初始化
	virtual void Init();

	//更新
	virtual void Play(float dt);

	//获取当前Brick状态
	virtual bool GetBrickState(int iRowIndex, int iColIndex);

	//获取小方块序列中的方块状态
	virtual bool GetSmallBrickState(int iRowIndex, int iColIndex);

	//获取游戏类型
	virtual SCENE_INDEX GetSceneType();

	//左按下
	virtual void OnLeftBtnPressed();

	//左释放
	virtual void OnLeftBtnReleased();

	//右按下
	virtual void OnRightBtnPressed();

	//右释放
	virtual void OnRightBtnReleased();

	//Fire按下
	virtual void OnFireBtnPressed();

	//Fire释放
	virtual void OnFireBtnReleased();

private:
	//初始化数据
	void initData();

	//移动
	bool bricksMoveDown(float dt);

	//随机状态
	bool randState();

	//检查结束
	void updateGameState();

	//射击方块
	bool fireBricks(float dt);

	//我方移动
	bool selfMove(float dt);

	//子弹移动
	bool bulletsMove(float dt);

	//消行
	void deleteLine(int nRowIdx);

private:
	enum VALID_BTN
	{
		VBTN_LEFT,
		VBTN_RIGHT,
		VBTN_FIRE,
		VBTN_MAX,
	};

	enum
	{
		GAMEOVER_WAIT_INTERVAL = 1500,		//游戏结束等待时间

		GAMEPASS_WAIT_INTERVAL = 1500,		//游戏通过等待时间

		BOOM_SHOWCOUNT = 16,				//闪烁显示爆炸效果次数

		BRICKS_MOVE_INTERVAL = 2500,		//方块移动时间间隔

		MOVE_CHECK_INTERVAL = 65,			//按钮检查时间间隔

		FIRE_CHECK_INTERVAL = 100,			//射击按钮检查时间间隔

		BULLET_MOVE_INTERVAL = 70,			//子弹移动时间
	};

	typedef list<POSITION>	TLIST_POS;
	typedef TLIST_POS::iterator	TLIST_POS_ITER;

private:
	bool m_arrBrick[ROW_NUM][COLUMN_NUM];		//方块状态序列

	POSITION m_stSelfPos;						//我方所在位置

	bool m_arrBtnPressFlag[DIR_MAX];			//按钮按下标记

	GAME_STATE	m_enGameState;					//游戏状态

	float m_fWaitTime;							//等待时间

	float m_fFireBtnTime;						//发射等待时间

	float m_fSelfMoveTime;						//移动等待时间

	TLIST_POS m_lsBullets;						//子弹列表

	float m_fBulletMoveTime;					//子弹移动等待时间

	bool m_bSpecialMode;						//特殊模式

	int m_iSpeed;								//速度

	int m_iLevel;								//关卡

	int m_iScore;								//分数

	int m_iLife;								//生命
};

