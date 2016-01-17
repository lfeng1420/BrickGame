#pragma once
#include "GameBase.h"
class CGameOver : public CGameBase
{
public:
	CGameOver();
	~CGameOver();

	//---------------------    GameBase    ----------------------
	//初始化
	void Init();

	//播放结束效果
	bool Play();

	//获取当前要更新Brick的位置
	void GetCurPos(int& iRowIndex, int& iColIndex);

	//游戏类型
	SCENE_INDEX GetSceneType();

	//获取每次执行完Play后等待的时间
	float GetRefreshTime();
	//---------------------    GameBase    ----------------------

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

