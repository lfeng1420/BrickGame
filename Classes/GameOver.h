#pragma once
#include "GameBase.h"
class CGameOver : public CGameBase
{
public:
	CGameOver();
	~CGameOver();

	//播放结束效果
	bool Play();

	//获取当前要更新Brick的位置
	void GetCurPos(int& iRowIndex, int& iColIndex);

	//游戏类型
	GAME_INDEX GetGameType();

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

