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

	//生命数
	bool GetSmallBrickState(int iRowIndex, int iColIndex);

	//获取游戏类型
	SCENE_INDEX GetSceneType();

	//左按下
	virtual void OnLeftBtnPressed();

	//右按下
	virtual void OnRightBtnPressed();

	//上按下
	virtual void OnUpBtnPressed();

	//下按下
	virtual void OnDownPressed();

	//Fire按下
	virtual void OnFireBtnPressed();

	//Fire释放
	virtual void OnFireBtnReleased();

	//---------------------    CSceneBase    ----------------------

private:
	//更新Brick，添加新行
	void UpdateBricks();
	
	//画赛车，iRoadIdx表示车道索引，iRowIdx表示行索引
	void RandCreateCars();

	//初始化Brick
	void InitBrick();

	//获取随机值
	void RandSeed();

	//获取等待时长
	int GetWaitInterval();

private:
	enum 
	{
		CAR_MAXNUM = 3,				//同时出现的车最大数量

		ROAD_COUNT = 4,				//车道数量

		CAR_WIDTH = 3,				//车宽度

		CAR_HEIGHT = 4,				//车高度

		GAMEOVER_REMAINTIME = 3000,	//游戏结束画面持续3秒

		ROW_DISTANCE = 11,			//超过11行生成新的赛车

		DEFAULT_INTERVAL = 160,		//默认等待时间

		CAR_DEFAULTROW = 2,			//默认所在行

		BOOM_SHOWCOUNT = 6,			//闪烁显示爆炸效果次数
	};


private:
	int m_iCarPos;								//车当前位置

	int m_iSpeed;								//速度

	int m_iLevel;								//关卡

	bool m_arrCurBrick[ROW_NUM][COLUMN_NUM];	//Brick状态

	int m_iLife;								//剩余生命

	int m_iRoadSignCount;						//路标行数

	bool m_bFirstShow;							//是否是第一次显示

	int m_fWaitTime;							//当前等待更新时间

	bool m_bImproveSpeed;						//是否加速

	bool m_bGameOver;							//是否游戏结束

	bool m_bShowBoom;							//爆炸显示/隐藏标记（闪烁）

	int m_iShowBoomCount;						//闪烁显示爆炸效果次数

	int m_iScore;								//分数

	int m_arrRowIdx[2];							//行记录
};

