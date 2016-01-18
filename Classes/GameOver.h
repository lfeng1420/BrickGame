#pragma once
#include "SceneBase.h"

class CGameOver : public CSceneBase
{
public:
	CGameOver(CGameScene* pGameScene);
	~CGameOver();

	//---------------------    SceneBase    ----------------------
	//初始化
	void Init();

	//更新
	void Play(float dt);

	//获取当前要更新Brick的位置
	void GetCurPos(int& iRowIndex, int& iColIndex);

	//游戏类型
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

	//开始
	void OnStart();
	//---------------------    SceneBase    ----------------------

private:
	int m_iColIdx;
	int m_iRowIdx;

	int m_iBeginColIdx;
	int m_iBeginRowIdx;

	int m_iEndColIdx;
	int m_iEndRowIdx;

	int m_iDirection;

	int m_iAllCount;
};

