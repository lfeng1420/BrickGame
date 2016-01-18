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
	log("Error: %s Current function is not implemented in CSceneBase class.", __FUNCTION__);
	return false;
}


bool CSceneBase::GetSmallBrickState(int iRowIndex, int iColIndex)
{
	log("Error: %s Current function is not implemented in CSceneBase class.", __FUNCTION__);
	return false;
}


//初始化
void CSceneBase::Init()
{
	
}


//左
void CSceneBase::OnLeft()
{
	log("Error: %s Current function is not implemented in CSceneBase class.", __FUNCTION__);
}

//右
void CSceneBase::OnRight()
{
	log("Error: %s Current function is not implemented in CSceneBase class.", __FUNCTION__);
}

//上
void CSceneBase::OnUp()
{
	log("Error: %s Current function is not implemented in CSceneBase class.", __FUNCTION__);
}

//下
void CSceneBase::OnDown()
{
	log("Error: %s Current function is not implemented in CSceneBase class.", __FUNCTION__);
}

//Fire
void CSceneBase::OnFire()
{
	log("Error: %s Current function is not implemented in CSceneBase class.", __FUNCTION__);
}

//开始
void CSceneBase::OnStart()
{
	log("Error: %s Current function is not implemented in CSceneBase class.", __FUNCTION__);
}

