#include "TankGame.h"

const int CORNER_POS[] = { 0, 0, 0, COLUMN_NUM - 1, ROW_NUM - 1, 0, ROW_NUM - 1, COLUMN_NUM - 1 };

//坦克四个方向状态
const int TANK_POSLIST[DIR_MAX][3][3] = 
{
	{	//DIR_RIGHT
		{ true, true, false },
		{ false, true, true },
		{ true, true, false },
	},
	{	//DIR_DOWN
		{ true, false, true },
		{ true, true, true },
		{ false, true, false },
	},
	{	//DIR_LEFT
		{ false, true, true },
		{ true, true, false },
		{ false, true, true },
	},
	{	//DIR_UP
		{ false, true, false },
		{ true, true, true },
		{ true, false, true },
	},
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
	m_iCurTime += dt;
	if (m_iCurTime < TANK_REFRESHTIME)
	{
		return;
	}

	//重置时间
	m_iCurTime = 0;

	CreateTank();

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

		bool bPosValid = false;
		for (int i = 0; i < sizeof(CORNER_POS) / sizeof(int) - 1; i += 2)
		{
			pTankData.m_stTankPos.m_iRowIdx = CORNER_POS[i];
			pTankData.m_stTankPos.m_iColIdx = CORNER_POS[i + 1];
			if (CheckPosValid(pTankData.m_stTankPos))
			{
				bPosValid = true;
				break;
			}
		}
		
		if (!bPosValid)
		{
			return;
		}

		//初始化坦克
		pTankData.m_bDead = false;
		pTankData.m_iCurStep = 0;
		pTankData.m_iMaxStep = Random(1, TANK_MAXSTEP);
		pTankData.m_iCurTime = 0;
		pTankData.m_iNextFireTime = Random(1, TANK_FIREMAXTIME);
		pTankData.m_iDirection = Random(DIR_MIN, DIR_MAX);			//随机方向

		//创建下一个
		++i;
	}
}


bool CTankGame::CheckPosValid(const TANK_POS& stDestPos)
{
	for (int i = 0; i < TANK_MAXNUM; ++i)
	{
		if (m_arrTankList[i].m_bDead)
		{
			continue;
		}

		TANK_DATA& pData = m_arrTankList[i];
		
		

	}

	return true;
}


//更新坦克序列
void CTankGame::TanksMove()
{
	for (int i = 0; i < TANK_MAXNUM; ++i)
	{
		if (m_arrTankList[i].m_bDead)
		{
			continue;
		}

		UpdateTankPos(i);
	}
}


//更新坦克位置
void CTankGame::UpdateTankPos(int iTankIdx)
{
	TANK_DATA& pData = m_arrTankList[iTankIdx];
	if (pData.m_iMaxStep > 0 && pData.m_iCurStep < pData.m_iMaxStep)
	{
		
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