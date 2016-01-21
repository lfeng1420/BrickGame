#include "FroggerGame.h"


CFroggerGame::CFroggerGame(CGameScene* pGameScene) : CSceneBase(pGameScene)
{
}


CFroggerGame::~CFroggerGame()
{
}


//初始化
void CFroggerGame::Init()
{
	//获取在选择游戏界面设置的速度和等级
	m_iSpeed = GET_INTVALUE("SPEED", 0);
	m_iLevel = GET_INTVALUE("LEVEL", 0);

	//默认生命数为4
	m_iLife = GET_INTVALUE("LIFE", 4);

	//初始化当前分数，经过的赛车数量
	m_iScore = 0;

	//更新界面，分数、等级和生命
	m_pGameScene->UpdateScore(m_iScore);
	m_pGameScene->UpdateLevel(m_iLevel);
	m_pGameScene->UpdateSmallBricks();

	InitData();
}

//更新
void CFroggerGame::Play(float dt)
{
	m_pGameScene->UpdateBricks();
}

//获取当前Brick状态
bool CFroggerGame::GetBrickState(int iRowIndex, int iColIndex)
{
	return false;
}

//生命数
bool CFroggerGame::GetSmallBrickState(int iRowIndex, int iColIndex)
{
	return false;
}

//获取游戏类型
SCENE_INDEX CFroggerGame::GetSceneType()
{
	return SCENE_FROGGER;
}

//左按下
void CFroggerGame::OnLeftBtnPressed()
{

}

//右按下
void CFroggerGame::OnRightBtnPressed()
{

}

//上按下
void CFroggerGame::OnUpBtnPressed()
{

}

//下按下
void CFroggerGame::OnDownPressed()
{

}

//Fire按下
void CFroggerGame::OnFireBtnPressed()
{

}


//更新所有河道
void CFroggerGame::UpdateRivers(float dt)
{
	for (int i = ROW_NUM - 2; i >= m_iEndRiverIdx; --i)
	{
		//偶数河道默认全部填充，无需更新
		if (i % 2 == 0)
		{
			continue;
		}

		UpdateRiver(i, dt);
	}
}

//更新指定行的河道
void CFroggerGame::UpdateRiver(int iRowIndex, float dt)
{
	RIVER& stRiver = m_mapRiver[iRowIndex];
	
	//更新时间
	stRiver.iCurTime += dt;

	//判断是否到了更新时间
	if (stRiver.iCurTime >= stRiver.arrRefreshTime[stRiver.iCurState])
	{
		//重置时间
		stRiver.iCurTime = 0;

		//检查方向
		if (stRiver.bLeft)
		{
			//向左移动，往最右边插入新Brick
			for (int i = 0; i < COLUMN_NUM - 1; ++i)
			{
				m_arrBrickState[iRowIndex][i] = m_arrBrickState[iRowIndex][i + 1];
			}
			m_arrBrickState[iRowIndex][COLUMN_NUM - 1] = stRiver.iCurState;
		}
		else
		{
			//向右移动，往最左边插入新Brick
			for (int i = COLUMN_NUM - 1; i > 0; --i)
			{
				m_arrBrickState[iRowIndex][i] = m_arrBrickState[iRowIndex][i - 1];
			}
			m_arrBrickState[iRowIndex][0] = stRiver.iCurState;
		}

		//计数增加，如果达到上限，则设置CurState
		++stRiver.iCount;
		if (stRiver.iCount == stRiver.arrMaxCount[stRiver.iCurState])
		{
			stRiver.iCurState = !stRiver.iCurState;
			//重置计数
			stRiver.iCount = 0;
		}
	}
}


//初始化数据、变量等
void CFroggerGame::InitData()
{
	//设置初始所在位置
	m_iSelfRowIdx = ROW_NUM - 1;
	m_iSelfColIdx = COLUMN_NUM / 2 - 1;

	//初始河道行上限
	m_iEndRiverIdx = ROW_NUM / 2;

	//初始化各个河道状态数据

}
