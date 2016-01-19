#include "SceneBase.h"

CSceneBase::CSceneBase(CGameScene* pGameScene) : m_pGameScene(pGameScene)
{

}


CSceneBase::~CSceneBase()
{
}

//更新
void CSceneBase::Play(float dt)
{

}


void CSceneBase::GetCurPos(int& iRowIndex, int& iColIndex)
{
	log("Error: %s Current function is not implemented in CSceneBase class.", __FUNCTION__);
}


SCENE_INDEX CSceneBase::GetSceneType()
{
	log("Error: %s Current function is not implemented in CSceneBase class.", __FUNCTION__);
	return SCENE_INVALID;
}


//获取当前Brick状态
bool CSceneBase::GetBrickState(int iRowIndex, int iColIndex)
{
	return false;
}


bool CSceneBase::GetSmallBrickState(int iRowIndex, int iColIndex)
{
	return false;
}


//初始化
void CSceneBase::Init()
{
	
}


//左
void CSceneBase::OnLeftBtnPressed()
{
	
}

void CSceneBase::OnLeftBtnReleased()
{
	
}

//右
void CSceneBase::OnRightBtnPressed()
{
	
}

void CSceneBase::OnRightBtnReleased()
{

}

//上
void CSceneBase::OnUpBtnPressed()
{
	
}

void CSceneBase::OnUpBtnReleased()
{

}

//下
void CSceneBase::OnDownPressed()
{
	
}

void CSceneBase::OnDownReleased()
{

}

//Fire
void CSceneBase::OnFireBtnPressed()
{
	
}

void CSceneBase::OnFireBtnReleased()
{

}

//开始
void CSceneBase::OnStart()
{
	
}

