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

	//初始化当前分数
	m_iScore = 0;

	//更新界面，分数、等级和生命
	m_pGameScene->UpdateScore(m_iScore, false);
	m_pGameScene->UpdateLevel(m_iLevel);
	m_pGameScene->UpdateSmallBricks();

	//初始河道状态数据
	const bool RIVER_DEFAULT_STATE[5][20] =
	{
		{ false, true, true, true, false, false, true, true, true, false, false, true, true, true, false, false, true, true, true, false },
		{ true, true, true, false, false, false, false, false, true, true, true, true, true, false, false, false, false, false, true, true },
		{ false, false, true, true, true, false, false, true, true, false, false, false, true, true, true, false, false, true, true, false },
		{ false, false, false, false, true, true, true, true, false, false, false, false, false, false, true, true, true, true, false, false },
		{ false, false, true, true, true, false, false, false, false, false, false, false, true, true, true, false, false, false, false, false },
	};

	//初始化河道数据
	bool bLeft = false;
	for (int i = RIVER_ROWTOP_INDEX + 1, iRiverCount = 0; i < ROW_NUM - 1; i += 2)
	{
		RIVER& stRiver = m_mapRiverData[i];
		bLeft = !bLeft;
		stRiver.bLeft = bLeft;
		
		for (int j = 0; j < 20; ++j)
		{
			stRiver.arrDefaultState[j] = RIVER_DEFAULT_STATE[iRiverCount][j];
		}

		++iRiverCount;
	}

	//初始化通过数量
	m_iPassCount = 0;

	InitData();
}

//更新
void CFroggerGame::Play(float dt)
{
	if (m_enGameState == GAMESTATE_PASS)
	{
		m_fPassCurTime += dt;
		if (m_fPassCurTime < GAMEPASS_WAITTIME)
		{
			return;
		}

		//重置通过数量
		m_iPassCount = 0;

		//更新速度和等级
		if (++m_iSpeed > 10)
		{
			m_iSpeed = 0;
			if (++m_iLevel > 10)
			{
				m_iLevel = 0;
			}
		}

		//更新显示
		m_pGameScene->UpdateLevel(m_iLevel);
		m_pGameScene->UpdateSpeed(m_iSpeed);

		//重置数据
		InitData();

		//界面更新
		m_pGameScene->UpdateBricks();
		return;
	}

	//进行中
	if (m_enGameState == GAMESTATE_RUNNING)
	{
		//更新自己
		bool bUpdateFlag = UpdateSelf(dt);

		//更新河道
		bUpdateFlag = UpdateRivers(dt) || bUpdateFlag;

		if (!bUpdateFlag)
		{
			return;
		}

		//更新游戏状态
		UpdateGameState();
	}

	if (m_enGameState == GAMESTATE_OVER)
	{
		if (!SetBoom(dt))
		{
			return;
		}

		if (m_iShowBoomCount >= BOOM_SHOWCOUNT)
		{
			//设置剩余生命
			--m_iLife;
			m_pGameScene->UpdateSmallBricks();
			
			//检查是否有剩余生命，没有则返回游戏结束界面
			if (m_iLife == 0)
			{
				m_pGameScene->RunScene(SCENE_GAMEOVER);
				return;
			}

			//重置数据
			InitData();

			m_pGameScene->UpdateSmallBricks();
		}
	}

	//界面更新
	m_pGameScene->UpdateBricks();
}

//获取当前Brick状态
bool CFroggerGame::GetBrickState(int iRowIndex, int iColIndex)
{
	//仅在游戏运行时显示自己的位置
	if (m_enGameState == GAMESTATE_RUNNING && iRowIndex == m_iSelfRowIdx && iColIndex == m_iSelfColIdx)
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


//更新所有河道
bool CFroggerGame::UpdateRivers(float dt)
{
	m_fRiverCurTime += dt;
	if (m_fRiverCurTime < DEFAULT_REFRESHTIME - 30 * m_iSpeed)
	{
		return false;
	}
	
	//重置
	m_fRiverCurTime = 0;

	//随机更新几个河道
	int iUpdateCount = 2 + rand() % (RIVER_COUNT - 2);
	bool arrUpdateFlag[RIVER_COUNT] = {false};
	for (int i = 0; i < iUpdateCount; )
	{
		int iIndex = Random(0, RIVER_COUNT);
		if (!arrUpdateFlag[iIndex])
		{
			arrUpdateFlag[iIndex] = true;
			++i;
		}
	}

	bool bNeedRefreshFlag = false;
	for (int i = RIVER_ROWTOP_INDEX + 1, iIndex = 0; i < ROW_NUM - 1; i += 2, ++iIndex)
	{
		if (arrUpdateFlag[iIndex])
		{
			UpdateRiver(i);
			bNeedRefreshFlag = true;
		}
	}

	return bNeedRefreshFlag;
}

//更新指定行的河道
void CFroggerGame::UpdateRiver(int iRowIndex)
{
	RIVER& stRiver = m_mapRiverData[iRowIndex];

	//更新
	if (stRiver.bLeft)
	{
		stRiver.iOffset += 1;
		if (stRiver.iOffset >= 20)
		{
			stRiver.iOffset = 0;
		}

		for (int i = stRiver.iOffset; i < 20; ++i)
		{
			if (i >= COLUMN_NUM + stRiver.iOffset)
			{
				return;
			}

			m_arrBrickState[iRowIndex][i - stRiver.iOffset] = stRiver.arrDefaultState[i];

		}

		for (int i = 0; i < COLUMN_NUM + stRiver.iOffset - 20; ++i)
		{
			m_arrBrickState[iRowIndex][20 - stRiver.iOffset + i] = stRiver.arrDefaultState[i];
		}
	}
	else
	{
		stRiver.iOffset -= 1;
		if (stRiver.iOffset <= -20)
		{
			stRiver.iOffset = 0;
		}

		int iStartIndex = stRiver.iOffset + 20;
		for (int i = iStartIndex; i < 20; ++i)
		{
			if (i >= COLUMN_NUM + iStartIndex)
			{
				return;
			}

			m_arrBrickState[iRowIndex][i - iStartIndex] = stRiver.arrDefaultState[i];
		}

		for (int i = 0; i < COLUMN_NUM + stRiver.iOffset; ++i)
		{
			m_arrBrickState[iRowIndex][i - stRiver.iOffset] = stRiver.arrDefaultState[i];
		}
	}
}


//更新自己
bool CFroggerGame::UpdateSelf(float dt)
{
	m_fSelfCurTime += dt;
	if (m_fSelfCurTime >= SELF_REFRESHTIME)
	{
		m_fSelfCurTime = 0;
		m_bSelfState = !m_bSelfState;
		return true;
	}

	return false;
}


//初始化数据、变量等
void CFroggerGame::InitData()

{
	//设置初始所在位置
	m_iSelfRowIdx = ROW_NUM - 1;
	m_iSelfColIdx = COLUMN_NUM / 2 - 1;

	//初始化青蛙的时间和状态
	m_fSelfCurTime = 0;
	m_bSelfState = false;

	//初始化爆炸状态
	m_iShowBoomCount = 0;
	m_fBoomCurTime = 0;

	//初始化河道当前时间
	m_fRiverCurTime = 0;

	//初始化游戏通过当前时间
	m_fPassCurTime = 0;

	//初始化游戏状态
	m_enGameState = GAMESTATE_RUNNING;

	//初始化各个河道状态数据
	int iRiverCount = 0;
	for (int i = ROW_NUM - 2; i >= RIVER_ROWTOP_INDEX; --i)
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
			RIVER& stRiver = m_mapRiverData[i];
			stRiver.iOffset = 0;

			//设置初始状态
			for (int j = 0; j < COLUMN_NUM; ++j)
			{
				m_arrBrickState[i][j] = stRiver.arrDefaultState[j];
			}

			++iRiverCount;
		}
	}

	//初始化其他行
	for (int i = RIVER_ROWTOP_INDEX - 1; i >= 0; --i)
	{
		for (int j = 0; j < COLUMN_NUM; ++j)
		{
			m_arrBrickState[i][j] = false;
		}
	}

	//最后一行
	for (int j = 0; j < COLUMN_NUM; ++j)
	{
		m_arrBrickState[ROW_NUM - 1][j] = false;
	}
}


//更新游戏状态：检查是否游戏结束
void CFroggerGame::UpdateGameState()
{
	if (m_arrBrickState[m_iSelfRowIdx][m_iSelfColIdx])
	{
		m_enGameState = GAMESTATE_OVER;
		PLAY_EFFECT(EFFECT_BOOM);
	}
}


//设置爆炸
bool CFroggerGame::SetBoom( float dt )
{
	m_fBoomCurTime += dt;
	if (m_fBoomCurTime < BOOM_REFRESHTIME)
	{
		return false;
	}

	//重置
	m_fBoomCurTime = 0;

	int iColStartIdx = m_iSelfColIdx;
	if (iColStartIdx > COLUMN_NUM - 4)
	{
		iColStartIdx = COLUMN_NUM - 4;
	}

	bool bShow = (m_iShowBoomCount % 2 == 0);
	for (int i = m_iSelfRowIdx - 3; i <= m_iSelfRowIdx; ++i)
	{
		int iIndex = i - (m_iSelfRowIdx - 3);
		for (int j = iColStartIdx; j < iColStartIdx + 4; ++j)
		{
			m_arrBrickState[i][j] = bShow && BOOM_STATE[iIndex][j - iColStartIdx];
		}
	}

	++m_iShowBoomCount;
	return true;
}


//左按下
void CFroggerGame::OnLeftBtnPressed()
{
	if (m_enGameState != GAMESTATE_RUNNING || m_iSelfColIdx == 0)
	{
		return;
	}

	//按钮音效
	PLAY_EFFECT(EFFECT_CHANGE2);

	//清除旧的位置
	m_pGameScene->UpdateBrick(m_iSelfRowIdx, m_iSelfColIdx, false, false);

	--m_iSelfColIdx;
	m_pGameScene->UpdateBrick(m_iSelfRowIdx, m_iSelfColIdx, false, true);

	//更新游戏状态
	UpdateGameState();
}

//右按下
void CFroggerGame::OnRightBtnPressed()
{
	if (m_enGameState != GAMESTATE_RUNNING || m_iSelfColIdx == COLUMN_NUM - 1)
	{
		return;
	}

	//按钮音效
	PLAY_EFFECT(EFFECT_CHANGE2);

	//清除旧的位置
	m_pGameScene->UpdateBrick(m_iSelfRowIdx, m_iSelfColIdx, false, false);

	++m_iSelfColIdx;
	m_pGameScene->UpdateBrick(m_iSelfRowIdx, m_iSelfColIdx, false, true);

	//更新游戏状态
	UpdateGameState();
}

//上按下
void CFroggerGame::OnUpBtnPressed()
{
	if (m_enGameState != GAMESTATE_RUNNING || m_iSelfRowIdx < RIVER_ROWTOP_INDEX)
	{
		return;
	}

	//按钮音效
	PLAY_EFFECT(EFFECT_CHANGE2);

	//清除旧的位置
	m_pGameScene->UpdateBrick(m_iSelfRowIdx, m_iSelfColIdx, false, false);

	m_iSelfRowIdx -= 2;
	m_pGameScene->UpdateBrick(m_iSelfRowIdx, m_iSelfColIdx, false, true);

	UpdateGameState();

	if (m_iSelfRowIdx == RIVER_ROWTOP_INDEX - 1 && m_enGameState != GAMESTATE_OVER)
	{
		//到达顶端的方块不再闪烁显示
		m_arrBrickState[m_iSelfRowIdx][m_iSelfColIdx] = true;

		//到达最顶端，分数增加
		m_iScore += 30;
		m_pGameScene->UpdateScore(m_iScore);

		//更新通过的青蛙数量
		++m_iPassCount;

		//检查是否通过关卡
		if (m_iPassCount == GAMEPASS_COUNT)
		{
			m_enGameState = GAMESTATE_PASS;
		}
		else
		{
			//重置位置和状态
			m_iSelfRowIdx = ROW_NUM - 1;
			m_iSelfColIdx = 6;
			m_fSelfCurTime = 0;
			m_bSelfState = true;
		}
	}
}

//下按下
void CFroggerGame::OnDownBtnPressed()
{
	if (m_enGameState != GAMESTATE_RUNNING || m_iSelfRowIdx >= ROW_NUM - 1)
	{
		return;
	}

	//按钮音效
	PLAY_EFFECT(EFFECT_CHANGE2);

	//清除旧的位置
	m_pGameScene->UpdateBrick(m_iSelfRowIdx, m_iSelfColIdx, false, false);

	m_iSelfRowIdx += 2;
	m_pGameScene->UpdateBrick(m_iSelfRowIdx, m_iSelfColIdx, false, true);

	//更新游戏状态
	UpdateGameState();
}

//Fire按下
void CFroggerGame::OnFireBtnPressed()
{
	OnUpBtnPressed();
}


void CFroggerGame::SaveGameData()
{
	
}