#include "TankGame.h"
#include "Tank.h"
#include "Bullet.h"
#include "TankCommon.h"

const int CORNER_POS[8] = 
{
	1, 1, 
	1, COLUMN_NUM - 2,
	ROW_NUM - 2, 1,
	ROW_NUM - 2, COLUMN_NUM - 2,
};

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
	TankMove(dt);

	//创建坦克
	CreateTank(dt);

	m_pGameScene->UpdateBricks();
}


//获取当前Brick状态
bool CTankGame::GetBrickState(int iRowIndex, int iColIndex)
{
	POSITION stTargetPos = { iRowIndex, iColIndex };

	//自己
	if (DrawTank(m_stSelfTank.m_stPos, m_stSelfTank.m_iDirection, stTargetPos))
	{
		return true;
	}

	//其他坦克
	for (int i = 0; i < TANK_MAXNUM; ++i)
	{
		if (m_arrTank[i].m_bDead)
		{
			continue;
		}

		const TANK_DATA& stData = m_arrTank[i];
		if (DrawTank(stData.m_stPos, stData.m_iDirection, stTargetPos))
		{
			return true;
		}
	}

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
	//初始化坦克移动等待时间
	m_fTankMoveTime = 0;
	m_fTankCreateTime = 0;

	m_iFirstCreateNum = 0;

	//初始化自己
	m_stSelfTank.m_bDead = false;
	m_stSelfTank.m_iCamp = CAMP_A;
	m_stSelfTank.m_fFireWaitTime = 0;
	m_stSelfTank.m_fFireMaxTime = Random(0, BULLET_CREATE_MAXTIME);
	m_stSelfTank.m_stPos = { ROW_NUM / 2 - 1 , COLUMN_NUM / 2 - 1};
	m_stSelfTank.m_iDirection = DIR_UP;

	//初始化子弹
	m_stSelfBullet.m_bValid = false;

	//初始化其他坦克和子弹
	for (int i = 0; i < TANK_MAXNUM; ++i)
	{
		//坦克
		m_arrTank[i].m_bDead = true;
		m_arrTank[i].m_iCamp = CAMP_B;
		m_arrTank[i].m_bNeedReset = false;

		//子弹
		m_arrBullet[i].m_bValid = false;
	}
}


//创建坦克
void CTankGame::CreateTank(float dt)
{
	m_fTankCreateTime += dt;
	if (m_fTankCreateTime < TANK_CREATE_TIME)
	{
		return;
	}

	m_fTankCreateTime = 0;

	int iTankIdx = 0;
	for (; iTankIdx < TANK_MAXNUM; ++iTankIdx)
	{
		if (!m_arrTank[iTankIdx].m_bDead)
		{
			continue;
		}

		TANK_DATA& stData = m_arrTank[iTankIdx];
		if (m_iFirstCreateNum < TANK_MAXNUM)
		{
			stData.m_stPos = { CORNER_POS[2 * m_iFirstCreateNum], CORNER_POS[2 * m_iFirstCreateNum + 1] };
			++m_iFirstCreateNum;
			break;
		}

		for (int j = 0; j < 8; j += 2)
		{
			stData.m_stPos = { CORNER_POS[j], CORNER_POS[j + 1] };
			if (CheckPos(iTankIdx, stData.m_stPos))
			{
				break;
			}
		}
	}

	if (iTankIdx >= TANK_MAXNUM)
	{
		return;
	}

	TANK_DATA& stData = m_arrTank[iTankIdx];

	//随机方向
	stData.m_iDirection = Random(DIR_MIN, DIR_MAX);

	//设置死亡标记
	stData.m_bDead = false;
	stData.m_iCurStep = 0;
	stData.m_iMaxStep = Random(0, TANK_MOVE_MAXSTEP);
	stData.m_fFireWaitTime = 0;
	stData.m_fFireMaxTime = Random(0, BULLET_CREATE_MAXTIME);
}


bool CTankGame::DrawTank(const POSITION& stPos, int iDirection, const POSITION& stTargetPos)
{
	for (int j = 0; j < 12; j += 2)
	{
		if (stPos.m_iRowIdx + TANK_POS_LIST[iDirection][j] == stTargetPos.m_iRowIdx
			&& stPos.m_iColIdx + TANK_POS_LIST[iDirection][j + 1] == stTargetPos.m_iColIdx)
		{
			return true;
		}
	}

	return false;
}


//检查位置是否有效
bool CTankGame::CheckPos(int iTankIdx, const POSITION& stPos)
{
	for (int i = 0; i < TANK_MAXNUM; ++i)
	{
		if (m_arrTank[i].m_bDead || (iTankIdx >= 0 && i == iTankIdx))
		{
			continue;
		}

		POSITION& stTankPos = m_arrTank[i].m_stPos;
		if (abs(stPos.m_iRowIdx - stTankPos.m_iRowIdx) < 3 && abs(stPos.m_iColIdx - stTankPos.m_iColIdx) < 3)
		{
			return false;
		}
	}

	if (iTankIdx >= 0)
	{
		POSITION& stSelfPos = m_stSelfTank.m_stPos;
		if (abs(stPos.m_iRowIdx - stSelfPos.m_iRowIdx) < 3 && abs(stPos.m_iColIdx - stSelfPos.m_iColIdx) < 3)
		{
			return false;
		}
	}

	return true;
}


//坦克移动
void CTankGame::TankMove(float dt)
{
	//时间更新
	m_fTankMoveTime += dt;
	if (m_fTankMoveTime < TANK_MOVE_INTERVAL)
	{
		return;
	}

	m_fTankMoveTime = 0;

	for (int i = 0; i < TANK_MAXNUM; ++i)
	{
		if (m_arrTank[i].m_bDead)
		{
			continue;
		}

		UpdateTankPos(i);
	}
}


//更新坦克位置
void CTankGame::UpdateTankPos(int iTankIdx)
{
	TANK_DATA& refData = iTankIdx < 0 ? m_stSelfTank : m_arrTank[iTankIdx];

	//是否需要重置步数和方向
	if (refData.m_bNeedReset || refData.m_iCurStep == refData.m_iMaxStep)	//可能随机到maxstep为0的情况
	{
		refData.m_iDirection = Random(DIR_MIN, DIR_MAX);
		refData.m_iCurStep = 0;
		refData.m_iMaxStep = Random(0, TANK_MOVE_MAXSTEP);
		refData.m_bNeedReset = false;
		return;
	}

	POSITION stNextPos;
	if (GetNextPos(iTankIdx, stNextPos))
	{
		if (CheckPos(iTankIdx, stNextPos))
		{
			//更新位置
			refData.m_stPos = stNextPos;

			//Step计数
			if (++refData.m_iCurStep >= refData.m_iMaxStep)
			{
				refData.m_bNeedReset = true;
			}
		}
		else
		{
			refData.m_bNeedReset = true;
		}
	}
	else
	{
		refData.m_bNeedReset = true;
	}

}

//获取下一个位置
bool CTankGame::GetNextPos(int iTankIdx, POSITION& stOutPos)
{
	const TANK_DATA& refData = iTankIdx < 0 ? m_stSelfTank : m_arrTank[iTankIdx];
	stOutPos = refData.m_stPos;
	switch (refData.m_iDirection)
	{
	case DIR_UP:
		--stOutPos.m_iRowIdx;
		break;
	case DIR_DOWN:
		++stOutPos.m_iRowIdx;
		break;
	case DIR_LEFT:
		--stOutPos.m_iColIdx;
		break;
	case DIR_RIGHT:
		++stOutPos.m_iColIdx;
		break;
	default:
		log("%s  Wrong Dirction Type. refData.m_iDirection=%d", __FUNCTION__, refData.m_iDirection);
		break;
	}

	if (stOutPos.m_iColIdx < 1 || stOutPos.m_iColIdx > COLUMN_NUM - 2
		|| stOutPos.m_iRowIdx < 1 || stOutPos.m_iRowIdx > ROW_NUM - 2)
	{
		return false;
	}

	return true;
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