#pragma once
#include "SceneBase.h"

class CRacingGame : public CSceneBase
{
public:
	CRacingGame();
	~CRacingGame();

	//---------------------    CSceneBase    ----------------------
	//初始化
	void Init();

	//游戏进行，返回false时表示游戏结束
	bool Play();

	//获取当前Brick状态
	bool GetBrickState(int iRowIndex, int iColIndex);

	//获取游戏类型
	SCENE_INDEX GetSceneType();

	//获取每次执行完Play后等待的时间
	float GetRefreshTime();

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
	int m_iCarPos;

};

