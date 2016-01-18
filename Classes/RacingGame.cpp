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
	m_iSpeed = GET_INTVALUE("Speed", 0);
	m_iLevel = GET_INTVALUE("Level", 0);

	//默认生命数为4
	m_iLife = 4;

	//初始化Brick
	for (int i = 0; i < ROW_NUM; ++i)
	{
		for (int j = 0; j < COLUMN_NUM; ++j)
		{
			m_arrBrickState[i][j] = false;
		}
	}
}

//更新
void CRacingGame::Play(float dt)
{
	m_pGameScene->UpdateBricks();
}

//获取当前Brick状态
bool CRacingGame::GetBrickState(int iRowIndex, int iColIndex)
{
	return m_arrBrickState[iRowIndex][iColIndex];
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

