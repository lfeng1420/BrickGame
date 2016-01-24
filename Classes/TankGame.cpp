#include "TankGame.h"

const int CORNER_POS[] = { 1, 1, 1, COLUMN_NUM - 2, ROW_NUM - 2, 1, ROW_NUM - 2, COLUMN_NUM - 2 };

/*
const TANK_POS TANK_POSLIST[DIR_MAX][6] = 
{
	{ TANK_POS(-1, -1), TANK_POS(-1, 0), TANK_POS(0, 0), TANK_POS(0, 1), TANK_POS(1, -1), TANK_POS(1, 0), },
	{ TANK_POS(-1, -1), TANK_POS(-1, 1), TANK_POS(0, -1), TANK_POS(0, 0), TANK_POS(0, 1), TANK_POS(1, 0), },
	{ TANK_POS(-1, 0), TANK_POS(-1, 1), TANK_POS(0, -1), TANK_POS(0, 0), TANK_POS(1, 0), TANK_POS(1, 1), },
	{ TANK_POS(-1, 0), TANK_POS(0, -1), TANK_POS(0, 0), TANK_POS(0, 1), TANK_POS(1, -1), TANK_POS(1, 1), },
};*/


const int TANK_POS_LIST[DIR_MAX][12] =
{
	{ -1, -1,   -1, 0,     0, 0,    0, 1,    1, -1,    1, 0 },
	{ -1, -1,   -1, 1,    0, -1,    0, 0,     0, 1,    1, 0 },
	{ -1, 0,    -1, 1,    0, -1,    0, 0,     1, 0,    1, 1 },
	{ -1, 0,    0, -1,     0, 0,    0, 1,    1, -1,    1, 1 },
};

CTankGame::CTankGame(CGameScene* pGameScene) : CSceneBase(pGameScene)
{
}


CTankGame::~CTankGame()
{
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

	InitData();
}


//更新
void CTankGame::Play(float dt)
{
	m_iCurTime += dt * 1000;
	if (m_iCurTime < TANK_REFRESH_TIME)
	{
		return;
	}

	//重置时间
	m_iCurTime = 0;

	//CreateTank();

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


void CTankGame::InitData()
{
	//初始化坦克自己所在的位置
	m_stSelf.m_iRowIdx = ROW_NUM / 2 - 1;
	m_stSelf.m_iColIdx = COLUMN_NUM / 2;

	//初始化坦克位置序列，设置已死亡即可
	for (int i = 0; i < TANK_MAXNUM; ++i)
	{
		m_arrTankList[i].m_bDead = true;
	}
	
	//初始化当前等待的时间
	m_iCurTime = 0;
}


//数量不足时创建坦克
void CTankGame::CreateTank()
{
	//保存死亡的坦克索引
	int arrDeadTankIdx[TANK_MAXNUM] = { -1 };

	int iTankCurCount = 0;
	for (int i = 0, iIndex = 0; i < TANK_MAXNUM; ++i)
	{
		if (!m_arrTankList[i].m_bDead)
		{
			++iTankCurCount;
		}
		else
		{
			arrDeadTankIdx[iIndex++] = i;
		}
	}

	//如果数量不够，创建新的坦克
	for (int i = iTankCurCount, iIndex = 0; i <= TANK_MAXNUM, iIndex < TANK_MAXNUM;)
	{
		if (arrDeadTankIdx[iIndex] == -1)
		{
			++iIndex;
			continue;
		}

		TANK_DATA& pTankData = m_arrTankList[iIndex];

		//找有效角落位置
		bool bPosValid = false;
		for (int i = 0; i < 8; i += 2)
		{
			pTankData.m_stTankPos.m_iRowIdx = CORNER_POS[i];
			pTankData.m_stTankPos.m_iColIdx = CORNER_POS[i + 1];
			pTankData.m_iDirection = Random(DIR_MIN, DIR_MAX);		//随机方向
			if (CheckPosValid(pTankData.m_stTankPos, pTankData.m_iDirection))
			{
				bPosValid = true;
				break;
			}
		}
		
		//如果没有找到有效位置就不创建坦克
		if (!bPosValid)
		{
			return;
		}

		//初始化坦克
		pTankData.m_bDead = false;
		pTankData.m_iCurStep = 0;
		pTankData.m_iMaxStep = Random(1, TANK_MOVE_MAXSTEP);
		pTankData.m_iCurTime = 0;
		pTankData.m_iNextFireTime = Random(0, TANK_FIRE_MAXTIME);
		pTankData.m_iRetryNum = 0;
		pTankData.m_iLastStep = -1;

		//创建下一个
		++i;
	}
}


//检查两个坦克位置是否有重叠
bool CTankGame::CheckTankOverlap(const TANK_POS& stSrcPos, int iSrcDir, const TANK_POS& stDestPos, int iDestDir)
{
	for (int i = 0; i < 12; i += 2)
	{
		int iSrcRowIdx = stSrcPos.m_iRowIdx + TANK_POS_LIST[iSrcDir][i];
		int iSrcColIdx = stSrcPos.m_iColIdx + TANK_POS_LIST[iSrcDir][i + 1];

		for (int j = 0; j < 12; j += 2)
		{
			int iDestRowIdx = stDestPos.m_iRowIdx + TANK_POS_LIST[iDestDir][j];
			int iDestColIdx = stDestPos.m_iColIdx + TANK_POS_LIST[iDestDir][j + 1];
			if (iSrcRowIdx == iDestRowIdx && iSrcColIdx == iDestColIdx)
			{
				return true;
			}
		}
	}

	return false;
}


//检查位置是否有效
bool CTankGame::CheckPosValid(const TANK_POS& stDestPos, int iDirection)
{
	for (int i = 0; i < TANK_MAXNUM; ++i)
	{
		if (m_arrTankList[i].m_bDead)
		{
			continue;
		}
		
		TANK_DATA& pData = m_arrTankList[i];
		if (CheckTankOverlap(stDestPos, iDirection, pData.m_stTankPos, pData.m_iDirection))
		{
			return false;
		}
	}

	return true;
}


//更新坦克序列
void CTankGame::UpdateTankPos()
{
	for (int i = 0; i < TANK_MAXNUM; ++i)
	{
		if (m_arrTankList[i].m_bDead)
		{
			continue;
		}

		TANK_DATA& pData = m_arrTankList[i];
		

	}
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