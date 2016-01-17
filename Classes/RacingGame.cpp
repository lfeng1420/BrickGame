#include "RacingGame.h"


CRacingGame::CRacingGame()
{
}


CRacingGame::~CRacingGame()
{
}


//初始化
void CRacingGame::Init()
{

}

//游戏进行，返回false时表示游戏结束
bool CRacingGame::Play()
{
	return true;
}

//获取当前Brick状态
bool CRacingGame::GetBrickState(int iRowIndex, int iColIndex)
{
	return true;
}

//获取游戏类型
SCENE_INDEX CRacingGame::GetSceneType()
{
	return GAME_RACING;
}

//获取每次执行完Play后等待的时间
float CRacingGame::GetRefreshTime()
{
	return 0;
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

