#include "SceneBase.h"


CSceneBase::CSceneBase()
{
}


CSceneBase::~CSceneBase()
{
}


bool CSceneBase::Play()
{
	return true;
}


void CSceneBase::GetCurPos(int& iRowIndex, int& iColIndex)
{
	
}


SCENE_INDEX CSceneBase::GetSceneType()
{
	return INVALID_SCENE;
}


//获取当前Brick状态
bool CSceneBase::GetBrickState(int iRowIndex, int iColIndex)
{
	return true;
}


//初始化
void CSceneBase::Init()
{

}


//获取每次执行完Play后等待的时间
float CSceneBase::GetRefreshTime()
{
	return 0;
}


//左
void CSceneBase::OnLeft()
{

}

//右
void CSceneBase::OnRight()
{

}

//上
void CSceneBase::OnUp()
{

}

//下
void CSceneBase::OnDown()
{

}

//Fire
void CSceneBase::OnFire()
{

}

//开始
void CSceneBase::OnStart()
{

}

