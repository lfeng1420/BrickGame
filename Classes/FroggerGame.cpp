#include "FroggerGame.h"

const bool RIVER_DEFAULT_STATE[5][20] =
{
	{ false, true, true, true, false, false, true, true, true, false, false, true, true, true, false, false, true, true, true, false},
	{ true, true, true, false, false, false, false, false, true, true, true, true, true, false, false, false, false, false, true, true },
	{ false, false, true, true, true, false, false, true, true, false, false, false, true, true, true, false, false, true, true, false },
	{ false, false, false, false, true, true, true, true, false, false, false, false, false, false, true, true, true, true, false, false },
	{ false, false, true, true, true, false, false, false, false, false, false, false, true, true, true, false, false, false, false, false },
};

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
	//更新自己
	UpdateSelf(dt);

	//河道更新
	UpdateRivers(dt);

	//界面更新
	m_pGameScene->UpdateBricks();
}

//获取当前Brick状态
bool CFroggerGame::GetBrickState(int iRowIndex, int iColIndex)
{
	if (iRowIndex == m_iSelfRowIdx && iColIndex == m_iSelfColIdx)
	{
		return m_bSelfState;
	}

	return m_arrBrickState[iRowIndex][iColIndex];
}

//生命数
bool CFroggerGame::GetSmallBrickState(int iRowIndex, int iColIndex)
{
	if (iRowIndex == 0 && iColIndex < m_iLife)
	{
		return true;
	}

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
	for (int i = ROW_NUM - 2; i >= RIVER_TOP_ROWINDEX; --i)
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
	RIVER& stRiver = m_mapRiverData[iRowIndex];
	stRiver.iCurTime += dt;

	//是否达到刷新时间
	if (stRiver.iCurTime >= stRiver.iRefreshTime)
	{
		stRiver.iCurTime = 0;

		//随机决定是否需要刷新
		srand((unsigned)time(nullptr));
		int iRandom = ((int)(CCRANDOM_0_1() * 100)) % 2;
		if (iRandom == 0)
		{
			return;
		}

		//更新
		if (stRiver.bLeft)
		{
			bool bTemp = m_arrBrickState[iRowIndex][0];
			for (int i = 1; i < COLUMN_NUM; ++i)
			{

			}
			stRiver.iOffset -= 1;
		}
		
	}
}


//更新自己
void CFroggerGame::UpdateSelf(float dt)
{
	m_iSelfCurTime += dt;
	if (m_iSelfCurTime >= SELF_REFRESHTIME)
	{
		m_iSelfCurTime = 0;
		m_bSelfState = !m_bSelfState;
	}
}


//初始化数据、变量等
void CFroggerGame::InitData()
{
	//设置初始所在位置
	m_iSelfRowIdx = ROW_NUM - 1;
	m_iSelfColIdx = COLUMN_NUM / 2 - 1;

	m_iSelfCurTime = 0;
	m_bSelfState = false;

	//初始化各个河道状态数据
	int iRiverCount = 0;
	bool bLeft = false;
	for (int i = ROW_NUM - 2; i >= RIVER_TOP_ROWINDEX; --i)
	{
		if (i % 2 == 0)
		{
			for (int j = 0; j < COLUMN_NUM; ++j)
			{
				m_arrBrickState[i][j] = true;
			}
		}
		else
		{
			bLeft = !bLeft;
			RIVER& stRiver = m_mapRiverData[i];
			stRiver.bLeft = bLeft;
			stRiver.iCurTime = 0;
			stRiver.iOffset = 0;
			stRiver.iRefreshTime = DEFAULT_REFRESHTIME - 30 * m_iSpeed;

			//设置初始状态
			for (int j = 0, int iCount = 0; j < COLUMN_NUM; ++j)
			{
				m_arrBrickState[i][j] = RIVER_DEFAULT_STATE[iRiverCount][iCount];
			}

			++iRiverCount;
		}
	}
}
