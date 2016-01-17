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

	//游戏进行，返回值表示当前状态
	void Play(float dt);

	//获取当前Brick状态
	bool GetBrickState(int iRowIndex, int iColIndex);

	//获取游戏类型
	SCENE_INDEX GetSceneType();

	//左
	void OnLeft();

	//右
	void OnRight();

	//上
	void OnUp();

	//下
	void OnDown();

	//Fire
	void OnFire();

	//---------------------    CSceneBase    ----------------------
private:
	enum 
	{
		CAR_MAXNUM = 3,		//同时出现的车最大数量

		ROAD_MAXINDEX = 3,	//车道最大索引，从0开始


	};


private:
	int m_iCarPos;			//车当前位置

	int m_iSpeed;			//速度

	int m_iLevel;			//关卡



};

