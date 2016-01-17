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


SCENE_INDEX CGameBase::GetSceneType()
{
	return INVALID_SCENE;
}


//获取当前Brick状态
bool CGameBase::GetBrickState(int iRowIndex, int iColIndex)
{
	return true;
}


//初始化
void CGameBase::Init()
{

}


//获取每次执行完Play后等待的时间
float CGameBase::GetRefreshTime()
{
	return 0;
}