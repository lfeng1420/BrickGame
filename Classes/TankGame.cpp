#include "TankGame.h"
#include "Tank.h"
#include "Bullet.h"
#include "TankCommon.h"

const int TANK_POS_LIST[DIR_MAX][12] =
{
	{ -1, -1, -1,  0,  0,  0,  0,  1,  1, -1,  1,  0 },
	{ -1, -1, -1,  1,  0, -1,  0,  0,  0,  1,  1,  0 },
	{ -1,  0, -1,  1,  0, -1,  0,  0,  1,  0,  1,  1 },
	{ -1,  0,  0, -1,  0,  0,  0,  1,  1, -1,  1,  1 },
};

CTankGame::CTankGame(CGameScene* pGameScene) : CSceneBase(pGameScene)
{
}


CTankGame::~CTankGame()
{
	for (int i = 0; i < 5; ++i)
	{
		CC_SAFE_DELETE(m_pArrTank[i]);
		CC_SAFE_DELETE(m_pArrBullet[i]);
	}
}


//初始化
void CTankGame::Init()
{
	//获取在选择游戏界面设置的速度和等级
	m_iSpeed = GET_INTVALUE("SPEED", 0);
	m_iLevel = GET_INTVALUE("LEVEL", 0);

	//默认生命数为4
	m_iLife = GET_INTVALUE("LIFE", 4);

	//初始化当前分数
	m_iScore = 0;

	//更新界面，分数、等级和生命
	m_pGameScene->UpdateScore(m_iScore);
	m_pGameScene->UpdateLevel(m_iLevel);
	m_pGameScene->UpdateSmallBricks();

	//坦克和子弹数组
	for (int i = 0; i < 5; ++i)
	{
		m_pArrTank[i] = new CTank();
		m_pArrBullet[i] = new CBullet();
	}

	InitData();
}


//更新
void CTankGame::Play(float dt)
{
	

}

//提供给每次更新单个Brick游戏（当前只有游戏结束）使用，获取当前改变的Brick行列索引
void CTankGame::GetCurPos(int& iRowIndex, int& iColIndex)
{

}

//获取当前Brick状态
bool CTankGame::GetBrickState(int iRowIndex, int iColIndex)
{
	return false;
}

//获取小方块序列中的方块状态
bool CTankGame::GetSmallBrickState(int iRowIndex, int iColIndex)
{
	return false;
}

//获取游戏类型
SCENE_INDEX CTankGame::GetSceneType()
{
	return SCENE_TANK;
}


//初始化数据
void CTankGame::InitData()
{
	//设置其他坦克无效，子弹无效
	for (int i = 0; i < 5; ++i)
	{
		if (i == 0)
		{
			//自己，初始化
			m_pArrTank[i]->Init(ROW_NUM / 2 - 1, COLUMN_NUM / 2 - 1, CAMP_A);
		}
		else
		{
			m_pArrTank[i]->SetDead(true);
		}

		m_pArrBullet[i]->SetValid(false);
	}
}


bool CTankGame::CheckTankPos(const TANK_POS& stSrcPos, int iSrcDir, const TANK_POS& stDestPos, int iDestDir)
{
	for (int i = 0; i < 12; i += 2)
	{
		int iSrcRowIdx = stSrcPos.m_iRowIdx + TANK_POS_LIST[iSrcDir][i];
		int iSrcColIdx = stSrcPos.m_iColIdx + TANK_POS_LIST[iSrcDir][i + 1];
		for (int j = 0; j < 12; j += 2)
		{
			int iDestRowIdx = stDestPos.m_iRowIdx + TANK_POS_LIST[iDestDir][i];
			int iDestColIdx = stDestPos.m_iColIdx + TANK_POS_LIST[iDestDir][i + 1];
			if (iDestRowIdx == iSrcRowIdx && iDestColIdx == iSrcColIdx)
			{
				return false;
			}
		}
	}

	return true;
}


//更新坦克位置
void CTankGame::UpdateTankPos()
{
	for (int i = 0; i < 5; ++i)
	{
		m_pArrTank[i]->UpdateTime();
		if (m_pArrTank[i]->CanMove())
		{
			
		}
	}
}


//创建坦克
void CTankGame::CreateTank()
{

}


//左按下
void CTankGame::OnLeftBtnPressed()
{

}

//左释放
void CTankGame::OnLeftBtnReleased()
{

}

//右按下
void CTankGame::OnRightBtnPressed()
{

}

//右释放
void CTankGame::OnRightBtnReleased()
{

}
//上按下
void CTankGame::OnUpBtnPressed()
{

}

//上释放
void CTankGame::OnUpBtnReleased()
{

}

//下按下
void CTankGame::OnDownPressed()
{

}

//下释放
void CTankGame::OnDownReleased()
{

}

//Fire按下
void CTankGame::OnFireBtnPressed()
{

}

//Fire释放
void CTankGame::OnFireBtnReleased()
{

}