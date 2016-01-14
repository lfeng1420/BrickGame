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