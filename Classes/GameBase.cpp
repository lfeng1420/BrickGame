#include "GameBase.h"


CGameBase::CGameBase()
{
}


CGameBase::~CGameBase()
{
}

bool CGameBase::Play()
{
	return true;
}


void CGameBase::GetCurPos(int& iRowIndex, int& iColIndex)
{
	
}


GAME_INDEX CGameBase::GetGameType()
{
	return INVALID_GAME;
}


//获取当前Brick状态
bool CGameBase::GetBrickState(int iRowIndex, int iColIndex)
{
	return true;
}

 