#pragma once
#include "SceneBase.h"

class CFlappyBirdGame : public CSceneBase
{
public:
	CFlappyBirdGame(CGameScene* pGameScene);
	~CFlappyBirdGame();

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
	
	//上按下
	void OnUpBtnPressed();

	//Fire按下
	void OnFireBtnPressed();

	//Fire释放
	void OnFireBtnReleased();

	//游戏状态
	void SaveGameData();

	//---------------------    CSceneBase    ----------------------

private:
	//初始化数据、变量等
	void InitData();

	//柱子移动
	bool PillarMove(float dt);

	//创建新柱子
	void CreatePillar();

	//鸟移动
	bool BirdMove(float dt);

	//更新游戏状态
	void UpdateGameState();

private:
	enum
	{
		PILLAR_COLUMN_DISTANCE = 6,		//柱子之间间隔6列

		PILLAR_MAXCOUNT = 3,			//柱子的最大数量

		PILLAR_MAXLEN = 12,				//柱子最大长度

		BIRD_DOWN_INTERVAL = 700,		//鸟移动移动时间间隔

		BIRD_DOWN_ACCELERATION = 10,	//鸟下降的加速度

		PILLAR_MOVE_INTERVAL = 500,		//柱子移动时间间隔

		BTN_CHECK_INTERVAL = 50,		//按钮检查间隔

		GAMEOVER_WAITTIME = 2000,		//游戏结束等待时间

		PILLAR_PASS_ADD_SCORE = 10,		//通过一根柱子加10分

		UPDATE_SPEED_NEED_SCORE = 500,	//更新速度时所需分数
	};


	struct PILLAR
	{
		int m_iColIdx;			//所在列

		int m_iLen;				//长度
	};

private:
	int m_iLife;								//生命数

	int m_iSpeed;								//当前速度

	int m_iLevel;								//当前等级

	int m_iScore;								//当前分数

	int m_iBirdRowIdx;							//鸟所在的行

	bool m_bBirdVisible;						//鸟显示状态

	float m_fPillarMoveTime;					//柱子移动等待时间

	float m_fBirdMoveTime;						//鸟移动等待时间

	float m_fBirdTotalTime;						//等待时长

	float m_fRefreshTime;						//刷新等待时间

	PILLAR m_arrPillar[PILLAR_MAXCOUNT];		//柱子所在列

	int m_iMoveCount;							//移动的列数计数

	GAME_STATE m_enGameState;					//游戏状态

	bool m_bImproveSpeed;						//加速

	bool m_bCanAddScore;						//是否可以加分
};