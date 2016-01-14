#pragma once
#include "GlobalDef.h"

class CGameBase
{
public:
	CGameBase();
	~CGameBase();

	//游戏进行，返回false时表示游戏结束
	virtual bool Play();

	//提供给每次更新单个Brick游戏（当前只有游戏结束）使用，获取当前改变的Brick行列索引
	virtual void GetCurPos(int& iRowIndex, int& iColIndex);

	virtual GAME_INDEX GetGameType();
};

