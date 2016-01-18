#include "RacingGame.h"


CRacingGame::CRacingGame(CGameScene* pGameScene) : CSceneBase(pGameScene)
{
}


CRacingGame::~CRacingGame()
{
}


//初始化
void CRacingGame::Init()
{

}

//更新
void CRacingGame::Play(float dt)
{
	return ;
}

//获取当前Brick状态
bool CRacingGame::GetBrickState(int iRowIndex, int iColIndex)
{
	return true;
}


//获取游戏类型
SCENE_INDEX CRacingGame::GetSceneType()
{
	return SCENE_RACING;
}


//左
void CRacingGame::OnLeft()
{

}

//右
void CRacingGame::OnRight()
{

}

//上
void CRacingGame::OnUp()
{

}

//下
void CRacingGame::OnDown()
{

}

//Fire
void CRacingGame::OnFire()
{

}

